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
	CellColony			cells;
	std::vector<Part*>	parts;
	Box*				box;
	PartSpecs*			partSpecs;

	MeasureTwoBodyForce*			measureTwoBodyForce;
	
public:
	System();
	System(CellColony* cellsIn, Box* boxIn);
	System(CellColony* cellsIn, PartSpecs* partSpecsIn, Box* boxIn);
	System(CellColony* cellsIn, PartSpecs* partSpecsIn, PartSpecs* partSpecsMeasureIn, Box* boxIn);

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

	void computeForces(double dt);
	void updatePositions(double dt, bool update);

	void registerTwoBodyForceMeasurement(MeasureTwoBodyForce* m);
	void collect();
	bool cellsAreBroken() const;

	size_t getTotalNumOfParts();
	

	void   setTypeFriction(size_t i, double friction);
	double getTypeFriction(size_t i);


	// for debugging:
	void printCoord();
	void printCoordDiff();

private:
	void System::constructPartsVector();
	void System::resolveOverlaps();

	void computeOneBodyForces(Part* part1, const PartSpecs* specs);
	void computeTwoBodyForces(Part* part1, const Part* part2, const PartSpecs* specs);

	void resetVelocities();


	// TODO: make sense out of these constructors
	System(size_t n);
public:
	System(CellColony* cellsIn);
};



