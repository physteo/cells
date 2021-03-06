#pragma once

#include <iostream>

// for nan checking
#include <cmath>

// gls for random numbers
#include <assert.h>

#include "../particles/Cell.h"
#include "../box/Box.h"

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>

#include "../particles/PartSpecs.h"

#include "../particles/CellColony.h"

#include "../definitions.h"


// for sorting
#include <algorithm>

// voro++
//#include "../utils/voropp/voro++.hh"

extern gsl_rng *g_rng;

class System
{
public:
	CellColony				cells;
	std::vector<Part*>		parts;
	Box*					box;
	std::vector<PartSpecs*>	partSpecs;
	
	unsigned short	cycleLength;
	size_t			maxPredictedCells;
	size_t			cellCounter;
	
public:
	System(CellColony* cellsIn, Box* boxIn, size_t maxPredictedCellsIn);
	//System(CellColony* cellsIn, PartSpecs* partSpecsMeasureIn, Box* boxIn);

	void addPartSpecs(PartSpecs* partSpecsIn);
	void setCycleLength(unsigned short cycleLengthIn);

	void clearSubBoxes();
	void setSubBoxes();

	CellColony getColony() const { return cells; }

	void computeForces(size_t time, double dt);
	//void computeForcesVoronoi(size_t time, double dt);

	void updateStages(size_t time);	
	int updatePositions(size_t time, double dt, bool update);

	//void collect();
	bool cellsAreBroken() const;
	bool twinCells() const;

	void eraseRegionCells(double minX, double maxX, double minY, double maxY);
	void eraseDeadCells();
	void divideCells();
	void resizeCellColony(size_t maxCells) = delete; // deleted because of the issues with cell numbering arising when adding/erasing new cells

	size_t getTotalNumOfParts();
	inline size_t getNumberOfCells() { return cells.size(); }

	inline const std::vector<Part*>&	getParts() const { return parts; }
	inline const CellColony& getConstCellColony() const { return cells; }
	inline const Box&  getBox() const { return *box; }
	inline const PartSpecs& getPartSpecs(size_t i) const { return *partSpecs.at(i); }


	void   setTypeFriction(size_t i, double friction);
	double getTypeFriction(size_t stage, size_t i);

	void setPartSpecs(PartSpecs* partSpecsIn);

	// to eliFminate:
	void addVelocity(double vx, double vy, size_t cell, size_t type)
	{
		this->cells.addVelocity(vx, vy, cell, type);
	}

	// for debugging:
	void printCoord();
	void printCoordDiff();

private:
	void constructPartsVector();

	void computeOneBodyForces(Part* part1);
	void computeTwoBodyForces(Part* part1, const Part* part2);

	void resetVelocities();

	//x//size_t computeStage(size_t time, size_t n);
	//x//size_t computeStage(size_t time, Part* part);

#ifdef DD
	std::vector<double> sigmaSpeeds;
	void diameterDynamics(size_t init, double dt);
#endif


	// TODO: make sense out of these constructors
	System(size_t n);
	System(CellColony* cellsIn, size_t maxPredictedCellsIn);

};



