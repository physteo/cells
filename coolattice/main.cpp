#include "pythonInterface/wrappers.h"

#include <iostream>
#include <stddef.h>
#include <vector>
#include <ctime>      // random numbers
#include <random>     // random numbers
#include <sys/types.h>    // for getpid()
//#include <unistd.h>       // for getpid()

// openmp
#include "utils/myOpenmp.h"

// visuals
#ifdef VISUALS
#include "visuals/Display.h"
#include "visuals/LightDisplay.h"
#endif

// math 
#include <gsl/gsl_rng.h> // gls for random numbers
#include <cmath>


typedef unsigned short myType;
const int MYSIZE = sizeof(myType) * 8;


// ==== seed choice ==== // 
// mix function (ref: http://burtleburtle.net/bob/hash/evahash.html)
unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
	a = a - b;  a = a - c;  a = a ^ (c >> 13);
	b = b - c;  b = b - a;  b = b ^ (a << 8);
	c = c - a;  c = c - b;  c = c ^ (b >> 13);
	a = a - b;  a = a - c;  a = a ^ (c >> 12);
	b = b - c;  b = b - a;  b = b ^ (a << 16);
	c = c - a;  c = c - b;  c = c ^ (b >> 5);
	a = a - b;  a = a - c;  a = a ^ (c >> 3);
	b = b - c;  b = b - a;  b = b ^ (a << 10);
	c = c - a;  c = c - b;  c = c ^ (b >> 15);
	return c;
}

gsl_rng *g_rng = NULL;

#include "./system/System.h"
#include "math/Vector.h"
#include "particles/PartSpecs.h"

#include "definitions.h"
#include "./utils/Hdf5.h"
#include "particles/CellColony.h"

//#include "utils/Hdf5types.h"
#include "system/SystemTrajectory.h"


#include "utils/readFile.h"
#include <fstream>

//#include "pythonInterface/wrappers.h"

// TODO: can I avoid including cpp here? it seems I cannot with the python interface
#include "pythonInterface/wrappers.cpp"



int main(int argc, char* argv[])
{	
	using namespace boost::python;
	using namespace pywrapper;

	// random number generator seed **********************************************
	//TODO URGENT: change to proper seed.
	std::string seedStr = "2";//inputJson.at("SIMULATION").at("seed").dump();
#ifdef LINUX
	unsigned long int seed = mix(clock(), time(NULL), getpid());
#else
	unsigned long int seed = (seedStr == "") ? mix(clock(), time(NULL), 1) : std::stoul(seedStr);
#endif

	g_rng = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(g_rng, seed);
	// ***************************************************************************

	std::cout << "rd: " << H5F_ACC_RDONLY << std::endl;
	std::cout << "tr: " << H5F_ACC_TRUNC << std::endl;
	
	std::string currentExecName = argv[0];
	std::string inputFilePath;

	if (argc == 2) {
		inputFilePath = argv[1];
	}
	else if (argc == 1) {
		//old_input//inputFilePath = "C:/Users/matte/Desktop/scripts/script_load_cycle.py";
		//old_input//inputFilePath = "C:/Users/matte/Desktop/scripts/cycle-create-bf0.8-desync.py";
		//old_input//inputFilePath = "C:/Users/matte/Desktop/scripts/coarse_grained_tests.py";
		inputFilePath = "C:/Users/matte/Desktop/scripts/cycle-LJadh-nodiv-create-bf0.8-desync.py";
	}
	else if (argc > 2) {
		std::cerr << "maximum one input file." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string script = Miscellaneous::read_file(inputFilePath.c_str());
	
	try {
		PyImport_AppendInittab("Cellulae", &initCellulae);
		Py_Initialize();
		object main_module((
			handle<>(borrowed(PyImport_AddModule("__main__")))));
		object main_namespace = main_module.attr("__dict__");
		object cpp_module((handle<>(PyImport_ImportModule("Cellulae"))));
		main_namespace["Cellulae"] = cpp_module;
		
		// execute python script
		handle<> ignored( (PyRun_String(script.c_str(),
		Py_file_input,
		main_namespace.ptr(),
		main_namespace.ptr()))  );
	}
	catch (error_already_set)
	{
		PyErr_Print();
	}

#ifdef NOLINUX
	std::cin.get();
#endif
	return 0;
}






// json reading file
/*

int maino(int argc, char* argv[])
{

	// read input file ***************************************************************
	using json = nlohmann::json;
	json inputJson;
	try {
		readJsonInputFile(argc, argv, &inputJson);
	}
	catch (std::exception& exc)
	{
		std::cerr << exc.what() << std::endl;
		return EXIT_FAILURE;
	}
	// *******************************************************************************




	// parallel ***************************************************************
#ifdef OMP
	omp_set_dynamic(0);
	size_t numThreads = inputJson["SIMULATION"]["threads"];
	omp_set_num_threads(numThreads);
	std::cout << "Num processors " << omp_get_num_procs() << std::endl;
	std::cout << "Num of max threads " << omp_get_max_threads() << std::endl;
#pragma omp parallel
	{
		printf("Num of threads begin used: %d\n", omp_get_num_threads());
	}
#endif
	// ***********************************************************************




	// random number generator seed **********************************************
	std::string seedStr = inputJson.at("SIMULATION").at("seed").dump();
	unsigned long int seed = (seedStr == "") ? mix(clock(), time(NULL), 1) : std::stoul(seedStr);
	g_rng = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(g_rng, seed);
	// ***************************************************************************


	// simulation parameters **************************************************
	size_t steps = (size_t) inputJson["SIMULATION"]["steps"];
	double dt = (double)inputJson["SIMULATION"]["dt"];
	double Lc = (double)inputJson["SIMULATION"]["subBoxMultipleOfMaxSigma"];
	size_t frequencyUpdateLists = (size_t)inputJson["SIMULATION"]["frequencyUpdateSubBoxes"];
	size_t frequencySampleTraj = inputJson["SIMULATION"]["frequencySampleTrajectory"];
	std::string inputConfig      = inputJson["SIMULATION"]["inputConfig"];
	std::string outputConfig     = inputJson["SIMULATION"]["outConfig"];
	std::string inputTrajectory  = inputJson["SIMULATION"]["inputTrajectory"];
	std::string outputTrajectory = inputJson["SIMULATION"]["outTrajectory"];
	// **************************************************


	// system specs **************************************************
	double areaFraction =  (double) inputJson["SYSTEM"]["phi"];
	int numPerLineX;
	int numPerLineY;


	numPerLineX = inputJson["SYSTEM"]["partsInAxisX"];
	numPerLineY = inputJson["SYSTEM"]["partsInAxisY"];

	std::string systemType = inputJson["SYSTEM"]["Type"]["name"];
	std::unique_ptr<PartSpecs> partsSpecifications;
	

	double A = 0.0;
	double boxArea = 0.0;
	double maxSigma = 0.0;
	if (systemType == "SMTY")
	{
		double Rmax = inputJson["SYSTEM"]["Type"]["Rmax"];
		double kappa = inputJson["SYSTEM"]["Type"]["kappa"];
		double sigAA = inputJson["SYSTEM"]["Type"]["sigAA"];
		double sigBB = inputJson["SYSTEM"]["Type"]["sigBB"];
		double eps = inputJson["SYSTEM"]["Type"]["eps"];
		double cut = pow(2., 1. / 6.);

		partsSpecifications = std::unique_ptr<SMTYSpecs>(new SMTYSpecs{ 1.1,0.88,1.0,9.64832e-5,pow(2., 1. / 6.),4.0,0.482416,1.0 });
				
		A = 0.29 * Rmax * Rmax;
		maxSigma = std::max(sigAA, sigBB);
	}
	int N = numPerLineX * numPerLineY;
	boxArea = A * N / areaFraction;
	// square box:
	double boxLength = sqrt(boxArea);
	double subBoxLength = Lc * maxSigma;
	int subBoxes = floor(boxLength / subBoxLength);




	CellColony colony{};
	SystemTrajectory trajectory;

	size_t numberOfCells = 0;

	if (inputConfig != "")
	{
		if (inputTrajectory == "")
		{
			// load config
			Hdf5 inputFile{ inputConfig.c_str() , H5F_ACC_RDONLY };
			colony.load(&inputFile, "t0");
			inputFile.closeFile();
			trajectory.push_back(colony);
		}
		else {
			// cant load both config and traj
			return EXIT_FAILURE;
		}
	}
	else if (inputTrajectory != "")
	{
		// load trajectory
		Hdf5 inputFileTraj(inputTrajectory.c_str(), H5F_ACC_RDONLY);
		trajectory.load(&inputFileTraj, "trajectory");
		inputFileTraj.closeFile();
	}
	else // new config
	{
		double lx = boxLength / numPerLineX;
		double ly = boxLength / numPerLineY;
		
		double sigBB = inputJson["SYSTEM"]["Type"]["sigBB"];

		for (int x = 0; x < numPerLineX; x++)
		{
			for(int y = 0; y < numPerLineY; y++)
			{
				std::vector<Part> cell;
				Vector positionF{ (x + 0.5)*lx, (y + 0.5)*ly };
				Vector positionB = positionF + 
									Vector{ (lx - sigBB) * (2.0 * gsl_rng_uniform(g_rng)- 1.0),
											(ly - sigBB) * (2.0 * gsl_rng_uniform(g_rng) - 1.0) };
				cell.push_back( Part{positionF, Vector{0.0, 0.0},0,numberOfCells });
				cell.push_back( Part{positionB, Vector{0.0, 0.0},1,numberOfCells });
				colony.push_back(cell);
				numberOfCells++;
			}
		}
		std::cout << "Number of cells: " << colony.size() << std::endl;
		trajectory.push_back(colony);
	}


	

	Box simulationBox = { boxLength, boxLength, true, subBoxes, subBoxes };
	System xSystem{ &trajectory.back(), partsSpecifications.get(), &simulationBox};
	//xSystem.clearSubBoxes();
	//xSystem.setSubBoxes();
	
	// MD loop
	if (steps > 0) {
		for (size_t i = 0; i < steps; i++)
		{
			//std::cout << "step:" << i << std::endl;
			xSystem.computeForces(dt);


			bool updateLists = (i % (steps / (steps / frequencyUpdateLists)) == 0 && steps > 1);
			xSystem.updatePositions(dt, updateLists);

			if (i % (steps / 100) == 0 && steps > 1) {
				std::cout << i / ((double)steps) << std::endl;
			}

			if (i % (steps / (steps / frequencySampleTraj)) == 0 && steps > 1) {
				trajectory.push_back(xSystem.cells);
			}

		}
	}

	// outputs
	if (outputConfig != "")
	{
		Hdf5 outFile{ outputConfig.c_str() , H5F_ACC_TRUNC};
		// retrieve last colony
		colony = trajectory.back();
		colony.save(&outFile, "t0");
		// save box
		xSystem.box->save(&outFile, "box");

		// save system specs
		partsSpecifications->save(&outFile, "specs");
		outFile.closeFile();
	}

	if (outputTrajectory != "")
	{
		//save trajectory
		Hdf5 outFileTraj{ outputTrajectory.c_str() , H5F_ACC_TRUNC };
		trajectory.save(&outFileTraj, "trajectory");

		// save box
		xSystem.box->save(&outFileTraj, "box");

		// save system specs
		partsSpecifications->save(&outFileTraj, "specs");

		outFileTraj.closeFile();
	}



	visuals::Display display{ 1500, 1500, xSystem.box, xSystem.partSpecs, &trajectory, false };
	display.run(0, trajectory.size(), 1, 1);


	return 0;
}
void readJsonInputFile(int argc, char* argv[], nlohmann::json* j)
{
	// read json input file
	std::string currentExecName = argv[0];
	std::string inputFilePath;

	if (argc == 2) {
		inputFilePath = argv[1];
	}
	else if (argc == 1) {
		// default file
		inputFilePath = "C:/Users/matte/Desktop/testJson.json";
	}
	else if (argc > 2) {
		std::cerr << "maximum one input file." << std::endl;
		throw std::exception("Maximum one input file.");
	}

	std::ifstream i(inputFilePath);
	i >> *j;
	i.close();
};
*/