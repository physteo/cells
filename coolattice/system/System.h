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
#include "../utils/voropp/voro++.hh"

class MeasureOneBodyForce
{
public:
	PartSpecs*		partSpecsMeasure;
	OneBodyForce*   oneBodyForce;
	size_t			partID;

	size_t			forceID;
	std::vector< std::vector<Vector> > data; // contains the measurements

public:
	MeasureOneBodyForce(PartSpecs* partSpecsIn, size_t partIDin, size_t forceIDin)
	{
		partID = partIDin;
		partSpecsMeasure = partSpecsIn;
		oneBodyForce = partSpecsIn->oneBodyForces.at(partIDin).at(forceIDin); // TODO
	}
};


class MeasureTwoBodyForce : BinarySavable
{
public:
	PartSpecs*		partSpecsMeasure;
	TwoBodyForce*   twoBodyForce;
	size_t			partID1;
	size_t			partID2;
	bool			intra;
	size_t			forceID;
	std::vector<std::vector<Vector> > data;
public:
	MeasureTwoBodyForce(PartSpecs* partSpecsIn, size_t partID1in, size_t partID2in,
		bool intraIn, size_t forceIDin);

	bool load(Hdf5* file, const char* name) override;
	bool save(Hdf5* file, const char* name) const override;
};





extern gsl_rng *g_rng;

class System
{
public:
	CellColony				cells;
	std::vector<Part*>		parts;
	Box*					box;
	std::vector<PartSpecs*>	partSpecs;

	MeasureTwoBodyForce*			measureTwoBodyForce;
	
	unsigned short	cycleLength;
	size_t			maxPredictedCells;
	size_t			cellCounter;
	
public:
	System(CellColony* cellsIn, Box* boxIn, size_t maxPredictedCellsIn);
	//System(CellColony* cellsIn, PartSpecs* partSpecsMeasureIn, Box* boxIn);

	void addPartSpecs(PartSpecs* partSpecsIn);
	void setCycleLength(unsigned short cycleLengthIn);

	//void saveMeasureTwoBodyForce()
	//{
	//	Hdf5 file{ "C:/Users/matte/Desktop/testMeasure.h5", 2 };
	//	measureTwoBodyForce->save(&file, "CIL");
	//	file.closeFile();
	//}

	void resizePartsDataSlots(size_t slots);

	void clearSubBoxes();
	void setSubBoxes();

	CellColony getColony() const { return cells; }

	void computeForces(size_t time, double dt);
	void computeForcesVoronoi(size_t time, double dt);


	int updatePositions(size_t time, double dt, bool update);

	void registerTwoBodyForceMeasurement(MeasureTwoBodyForce* m);
	void collect();
	bool cellsAreBroken() const;
	bool twinCells() const;

	void eraseRegionCells(double minX, double maxX, double minY, double maxY);
	void eraseDeadCells();
	void duplicateCells();
	void resizeCellColony(size_t maxCells) = delete; // deleted because of the issues with cell numbering arising when adding/erasing new cells

	size_t getTotalNumOfParts();
	inline size_t getNumberOfCells() { return cells.size(); }

	inline const std::vector<Part*>&	getParts() const { return parts; }
	inline const CellColony& getConstCellColony() const { return cells; }
	inline const Box&  getBox() const { return *box; }
	inline const PartSpecs& getPartSpecs(size_t i) const { return *partSpecs.at(i); }
	inline const MeasureTwoBodyForce& getMeasureTwoBodyForce() const { return *measureTwoBodyForce; }


	void   setTypeFriction(size_t i, double friction);
	double getTypeFriction(size_t i);

	void setPartSpecs(PartSpecs* partSpecsIn);

	// to eliminate:
	void addVelocity(double vx, double vy, size_t cell, size_t type)
	{
		this->cells.addVelocity(vx, vy, cell, type);
	}

	// for debugging:
	void printCoord();
	void printCoordDiff();

private:
	void System::constructPartsVector();
	void System::resolveOverlaps();

	void computeOneBodyForces(Part* part1, const PartSpecs* specs);
	void computeTwoBodyForces(Part* part1, const Part* part2, const PartSpecs* specs);

	void resetVelocities();

	size_t computeStage(size_t time, size_t n);
	size_t computeStage(size_t time, Part* part);


	// TODO: make sense out of these constructors
	System(size_t n);
	System(CellColony* cellsIn, size_t maxPredictedCellsIn);

};



