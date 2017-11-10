#include <iostream>

#include <gsl/gsl_rng.h> // gls for random numbers
#include <cmath>
gsl_rng *g_rng = NULL;

#include "utils/voropp/voro++.hh"
#include "utils/myOpenmp.h"


// Set up constants for the container geometry
const double x_min = 0, x_max = 1;
const double y_min = 0, y_max = 1;
const double z_min = -1, z_max = 1;

 // Set up the number of blocks that the container is divided into
const int n_x = 10, n_y = 1, n_z = 1;

struct TestPart
{
	double x;
	double y;
	TestPart(double xIn, double yIn) : x(xIn), y(yIn) {}
};
#define NOBLOCKS
#define OMP
int main()
{
	std::cout << "Testing voro++" << std::endl;
	unsigned long seed = 1;
	g_rng = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(g_rng, seed);

	std::vector<TestPart> parts;
	size_t numParts = 10000;
	for (size_t i = 0; i < numParts; i++) {
		parts.emplace_back(TestPart{ gsl_rng_uniform(g_rng), gsl_rng_uniform(g_rng) });
	}

	//parts.emplace_back(TestPart{ 0.0, 0.0 });
	//parts.emplace_back(TestPart{ 1.0, 2.0 });
	//parts.emplace_back(TestPart{ 3.0, 0.5 });
	//parts.emplace_back(TestPart{ 0.2, 0.8 });
	//parts.emplace_back(TestPart{ -1.7, 3.4 });


	voro::container con(x_min, x_max, y_min, y_max, z_min, z_max, n_x, n_y, n_z,
	                         false, false, false, 8);


	for (int i = 0; i < parts.size(); i++)
		con.put(i, parts.at(i).x, parts.at(i).y, 0.0);



#ifdef BLOCKS
	int		blocksX		= con.nx;
	double	blockSizeX	= con.boxx;

#ifdef OMP
	myOpenmp omp{ 4 };
	omp.activate();
#pragma omp parallel for
#endif
	for (int b = 0; b < blocksX; b++)
	{
		voro::c_loop_subset loop = voro::c_loop_subset(con);
		loop.setup_intbox(b, b, 0, n_y, 0, n_z);
		
		int currentBlock = loop.ijk;
		voro::voronoicell_neighbor c;
		//std::cout << "block: " << b << std::endl;
		if (loop.start()) do if (con.compute_cell(c, loop)) {
			double x, y, z;
			int ID;
			loop.pos(x, y, z); // retrieve position of particle
			ID = loop.pid();   // retrieve ID of particle
			int block = loop.ijk;
			
			std::vector<int> neighbours;
			c.neighbors(neighbours);
			for (size_t j = 0; j < neighbours.size(); j++)
			{
				int ID = neighbours.at(j);
				//if(ID > 0)
				//	std::cout << "(" << parts.at(ID).x << ", " << parts.at(ID).y << ") - ";
			}
			//std::cout << std::endl;

		} while (loop.inc());
		
	}
	//con.draw_cells_gnuplot("C:/Users/matte/Desktop/scripts/voro/test1_cells_blocks.gnu");
	//con.draw_particles("C:/Users/matte/Desktop/scripts/voro/test1_parts_blocks.gnu");
	std::cout << "end" << std::endl;
#else


	for (int i = 0; i < parts.size(); i++)
	{
		voro::voronoicell_neighbor c;
		con.compute_cell(c, 0, i);
		
		std::vector<int> neighbours;
		c.neighbors(neighbours);
		
		for (size_t j = 0; j < neighbours.size(); j++)
		{
			int ID = neighbours.at(j);
			//if(ID > 0)
			//	std::cout << "(" << parts.at(ID).x << ", " << parts.at(ID).y << ") - ";
		}
		//std::cout << std::endl;
	}

	con.draw_cells_gnuplot("C:/Users/matte/Desktop/scripts/voro/test1_cells.gnu");
	con.draw_particles("C:/Users/matte/Desktop/scripts/voro/test1_parts.gnu");

	std::cout << "end" << std::endl;
#endif	

	

	std::cin.get();
	return 0;
}