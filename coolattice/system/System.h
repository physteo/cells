#pragma once

#include <iostream>

// for nan checking
#include <cmath>

// gls for random numbers
#include <assert.h>

#include "../particles/Cell.h"
#include "../box/Box.h"

#define FRICTION 1.0
#define MASS 1.0

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>

#include "../particles/PartSpecs.h"

#include "../particles/CellColony.h"

#include "../definitions.h"


extern gsl_rng *g_rng;

class System
{
public:
	//std::vector<Cell> cells;
	CellColony			cells;
	std::vector<Part*>	parts;
	Box*				box;
	PartSpecs*			partSpecs;

public:
	System();
	System(CellColony* cellsIn, Box* boxIn);
	System(CellColony* cellsIn, PartSpecs* partSpecsIn, Box* boxIn);


	void clearSubBoxes();
	void setSubBoxes();

	CellColony getColony() const { return cells; }
	void computeForces(double dt);
	void updatePositions(double dt, bool update);
	bool cellsAreBroken() const;

	size_t getTotalNumOfParts();
	
	// for debugging:
	void printCoord();
	void printCoordDiff();

private:
	void System::constructPartsVector();
	void System::resolveOverlaps();

	// TODO: make sense out of these constructors
	System(size_t n);
public:
	System(CellColony* cellsIn);

};