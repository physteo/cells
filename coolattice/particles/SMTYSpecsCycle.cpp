#include "SMTYSpecsCycle.h"

SMTYSpecsCycle::SMTYSpecsCycle() : PartSpecs(2)
{
	name = "SMTY-Cycle";
}

void SMTYSpecsCycle::build()
{

	const double& sigAA = this->partTypes.getPartTypes().at(0).sig;
	const double& sigBB = this->partTypes.getPartTypes().at(1).sig;
	const double& frictionF = this->partTypes.getPartTypes().at(0).friction;
	const double& frictionB = this->partTypes.getPartTypes().at(1).friction;


	double sigAB = .5*(sigAA + sigBB);

	// characteristics
	//double ssCellLength = sqrt(m_rMaxSquared) * sqrt(1.0 - m_kappa * (1.0 / frictionF + 1.0 / frictionB) / (m_motility / frictionF));
	//double ssSpeed = ssCellLength * m_motility / (frictionF + frictionB);
	//double migrationTime = sqrt(m_rMaxSquared) / ssSpeed;
	//std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	//std::cout << " - Cell Length = " << ssCellLength << std::endl;
	//std::cout << " - Spees at ss = " << ssSpeed << std::endl;
	//std::cout << " - Migration t = " << migrationTime << std::endl;

	LJForce* lja(new LJForce{ sigAA, m_epsilon, (sigAA *   sigAA * m_cut * m_cut) });
	LJForce* ljb(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
	LJForce* ljc(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
	LJForce* ljd(new LJForce{ sigBB, m_epsilon, (sigBB *   sigBB * m_cut * m_cut) });

	addInterForce(0, lja);
	addInterForce(1, ljb);
	addInterForce(2, ljc);
	addInterForce(3, ljd);

	FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });



	addIntraForce(0, feneAA);
	addIntraForce(1, feneAB);
	addIntraForce(2, feneBA);
	addIntraForce(3, feneBB);

	if (m_cycleStage == 0) {
		CilForce*  cil(new CilForce{ m_motility });
		addIntraForce(1, cil);
		addIntraForce(2, cil);
	}

	//else if (m_cycleStage == 1)
	//{
	//	CilForce*  cil(new CilForce{ m_motility });
	//	addIntraForce(1, cil);
	//	addIntraForce(2, cil);
	//
	//}

	//partSpecs.oneBodyForces.at(0).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7})))); // TODO URGENT: pass dt etc
	//partSpecs.oneBodyForces.at(1).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7 })))); // TODO URGENT: pass dt etc



	
	
	
	
}


SMTYSpecsCycle::SMTYSpecsCycle(double sigAA, double sigBB, double motility,
	double epsilon, double cut, double rMaxSquared, double kappa, double frictionF,
	double frictionB, double massF, double massB,
	double rateDuplication, double thresholdDuplication,
	size_t cycleStage, size_t cycleLength) : SMTYSpecsCycle()
{

	m_motility = motility;
	m_epsilon = epsilon;
	m_cut = cut;
	m_rMaxSquared = rMaxSquared;
	m_kappa = kappa;
	m_cycleStage = cycleStage;
	m_cycleLength = cycleLength;
	m_rateDuplication = rateDuplication;
	m_thresholdDuplication = thresholdDuplication;

	// particle types
	this->partTypes.getPartTypes().at(0).name = "F";
	this->partTypes.getPartTypes().at(1).name = "B";
	this->partTypes.getPartTypes().at(0).mass = massF;
	this->partTypes.getPartTypes().at(1).mass = massB;
	this->partTypes.getPartTypes().at(0).sig = sigAA;
	this->partTypes.getPartTypes().at(1).sig = sigBB;

	if (frictionF <= 0) {
		this->partTypes.getPartTypes().at(0).friction = std::numeric_limits<double>::infinity();
	}
	else
	{
		this->partTypes.getPartTypes().at(0).friction = frictionF;
	}

	if (frictionB <= 0) {
		this->partTypes.getPartTypes().at(1).friction = std::numeric_limits<double>::infinity();
	}
	else
	{
		this->partTypes.getPartTypes().at(1).friction = frictionB;
	}
	
	build();
}


SMTYSpecsCycle::SMTYSpecsCycle(const Parameters* params, size_t cycleStage, size_t cycleLength) : 
	SMTYSpecsCycle(	params->getParam(0),
					params->getParam(1),
					params->getParam(2),
					params->getParam(3),
					params->getParam(4),
					params->getParam(5),
					params->getParam(6),
					params->getParam(7),
					params->getParam(8),
					params->getParam(9),
					params->getParam(10),
					params->getParam(11),
					params->getParam(12),
					cycleStage, cycleLength)
{
}


bool SMTYSpecsCycle::load(Hdf5* file, const char* groupName)
{
	try {
		H5::Group group = file->openGroup(groupName);

		std::string readName = file->readAttributeString(groupName, "name");
		// check if name is the same
		if (readName != name)
		{
			std::cerr << "Error: Trying to load a '" << readName << "' SystemSpecs into a '" << name << "' SystemSpecs." << std::endl;
			exit(EXIT_FAILURE);
		}

		m_motility = file->readAttributeDouble(groupName, "motility");
		m_epsilon = file->readAttributeDouble(groupName, "eps");
		m_cut = file->readAttributeDouble(groupName, "cut");
		m_rMaxSquared = file->readAttributeDouble(groupName, "rMax2");
		m_kappa = file->readAttributeDouble(groupName, "k");
		m_rateDuplication = file->readAttributeDouble(groupName, "rateDuplication");
		m_thresholdDuplication = file->readAttributeDouble(groupName, "thresholdDuplication");
		m_cycleStage = file->readAttributeInteger(groupName, "cycleStage");
		m_cycleLength = file->readAttributeInteger(groupName, "cycleLength");

		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types");
		this->partTypes.load(file, partTypesGroupName);


		build();
		return true;
	}
	catch (H5::Exception &err)
	{
		err.printError(); // TODO: logger
		return false;
	}
	catch (std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		return false;
	}
}



bool SMTYSpecsCycle::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "motility", m_motility);
		file->writeAttributeDouble(groupName, "eps", m_epsilon);
		file->writeAttributeDouble(groupName, "cut", m_cut);
		file->writeAttributeDouble(groupName, "rMax2", m_rMaxSquared);
		file->writeAttributeDouble(groupName, "k", m_kappa);
		file->writeAttributeDouble(groupName, "rateDuplication", m_rateDuplication);
		file->writeAttributeDouble(groupName, "thresholdDuplication", m_thresholdDuplication);
		file->writeAttributeInteger(groupName, "cycleStage", m_cycleStage);
		file->writeAttributeInteger(groupName, "cycleLength", m_cycleLength);


		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types");
		this->partTypes.save(file, partTypesGroupName);

		return true;
	}
	catch (H5::Exception &err)
	{
		err.printError(); // TODO: logger
		return false;
	}
}



bool SMTYSpecsCycle::cellIsBroken(const Cell* cell, const Box* box) const
{
	// in the SMTY system a cell is broken when it's length is
	// bigger than Rmax
	Vector vectorDistance;
	double distance2 = box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, vectorDistance);
	if (distance2 > m_rMaxSquared)
	{
		std::cerr << "Broken cell detected." << std::endl;
		return true;
	}
	else if (std::isnan(distance2))
	{
		std::cerr << "Cell distance is NaN." << std::endl;
		return true;
	}

	return false;

}


bool SMTYSpecsCycle::cellIsDead(const Cell* cell, const Box* box) 
{
	return false;
}


bool SMTYSpecsCycle::cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
	// in this model, a cell can duplicate if its length is bigger than Rmax/2.0
	Vector vectorDistance;
	double distance2 = box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, vectorDistance);

	// a cell can duplicate once its length is bigger than a threshold (submultiple of Rmax)
	if (distance2 > m_rMaxSquared * m_thresholdDuplication * m_thresholdDuplication)
	{
		if (gsl_rng_uniform(g_rng) < m_rateDuplication) {
			

			// bla
			Cell newcell = Cell(2);
			newcell.getPart(0).type = 0;
			newcell.getPart(0).cell = cellCounter;

			newcell.getPart(0).position = cell->getPart(1).position;

			newcell.getPart(1).type = 1;
			newcell.getPart(1).cell = cellCounter;
			newcell.getPart(1).position = cell->getPart(1).position;

			// assign stage
			unsigned short cellStage = gsl_rng_uniform_int(g_rng, cycleLength);
			newcell.getPart(0).stage = cellStage;
			newcell.getPart(1).stage = cellStage;



			cell->getPart(1).position = cell->getPart(0).position;

			// set velocities to zero
			newcell.getPart(0).velocity = Vector{ 0.0, 0.0 };
			newcell.getPart(1).velocity = Vector{ 0.0, 0.0 };
			cell->getPart(0).velocity = Vector{ 0.0,0.0 };
			cell->getPart(1).velocity = Vector{ 0.0,0.0 };

			newCells->push_back(newcell);
			cellCounter++;
			return true;
		}
	}
	return false;
}