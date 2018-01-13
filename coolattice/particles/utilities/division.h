#pragma once
#include "../Cell.h"
#include "../../box/Box.h"

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>
extern gsl_rng *g_rng;


class DivisionMethods {

public:

	// standard division creates a new cell and place it on top of the Back Part of the old cell. Then it
	// moves the Back Part of the old cell on top of the Front Part of the old Cell. The two cells are given
	// zero velocity and are assigned a specific stage (the same stage)
	static bool standardDivision(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength, size_t stageNewCells, const PartTypeVector* partTypes)
	{
		Cell newcell = Cell(2);
		newcell.getPart(0).type = 0;
		newcell.getPart(0).cell = cellCounter;
		newcell.getPart(0).position = cell->getPart(1).position;

		newcell.getPart(1).type = 1;
		newcell.getPart(1).cell = cellCounter;
		newcell.getPart(1).position = cell->getPart(1).position;

		// set velocities to zero
		newcell.getPart(0).velocity = Vector{ 0.0, 0.0 };
		newcell.getPart(1).velocity = Vector{ 0.0, 0.0 };

		// assign stage
		unsigned short currentStageTime = gsl_rng_uniform_int(g_rng, cycleLength);
		newcell.getPart(0).currentStage = stageNewCells;
		newcell.getPart(1).currentStage = stageNewCells;
		newcell.getPart(0).currentStageTime = currentStageTime;
		newcell.getPart(1).currentStageTime = currentStageTime;
		newcell.getPart(0).currentSigma = partTypes->getPartTypes().at(0).sig;
		newcell.getPart(1).currentSigma = partTypes->getPartTypes().at(1).sig;

		newCells->push_back(newcell);
		cellCounter++;

		// modify old cell. Do not change its cell ID!
		cell->getPart(1).position = cell->getPart(0).position;
		cell->getPart(0).velocity = Vector{ 0.0,0.0 };
		cell->getPart(1).velocity = Vector{ 0.0,0.0 };
		return true;

	}



	static bool standardDivisionWithRandomDisplacement(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength, size_t stageNewCells, const PartTypeVector* partTypes)
	{
		Cell newcell = Cell(2);
		newcell.getPart(0).type = 0;
		newcell.getPart(0).cell = cellCounter;
		newcell.getPart(0).position = cell->getPart(1).position;

		newcell.getPart(1).type = 1;
		newcell.getPart(1).cell = cellCounter;
		newcell.getPart(1).position = cell->getPart(1).position;

		double dx = 0.01 * gsl_rng_uniform(g_rng);
		double dy = 0.01 * gsl_rng_uniform(g_rng);
		newcell.getPart(1).position += Vector{dx, dy};



		// set velocities to zero
		newcell.getPart(0).velocity = Vector{ 0.0, 0.0 };
		newcell.getPart(1).velocity = Vector{ 0.0, 0.0 };

		// assign stage
		unsigned short currentStageTime = gsl_rng_uniform_int(g_rng, cycleLength);
		newcell.getPart(0).currentStage = stageNewCells;
		newcell.getPart(1).currentStage = stageNewCells;
		newcell.getPart(0).currentStageTime = currentStageTime;
		newcell.getPart(1).currentStageTime = currentStageTime;
		newcell.getPart(0).currentSigma = partTypes->getPartTypes().at(0).sig;
		newcell.getPart(1).currentSigma = partTypes->getPartTypes().at(1).sig;

		newCells->push_back(newcell);
		cellCounter++;

		// modify old cell. Do not change its cell ID!
		cell->getPart(1).position = cell->getPart(0).position;
		cell->getPart(0).velocity = Vector{ 0.0,0.0 };
		cell->getPart(1).velocity = Vector{ 0.0,0.0 };

		// random displ
		dx = 0.01 * gsl_rng_uniform(g_rng);
		dy = 0.01 * gsl_rng_uniform(g_rng);
		cell->getPart(1).position += Vector{ dx, dy };


		return true;

	}


};