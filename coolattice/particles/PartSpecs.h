#pragma once

#include <vector>
#include <assert.h>
#include <memory>
#include <string.h> // for strcmp

#include "../math/Vector.h"
#include "PartType.h"

#include "../interactions/Force.h"
#include "../interactions/TwoBodyForce.h"

#include "../interactions/LJForce.h"
#include "../interactions/FeneForce.h"
#include "../interactions/GaussianRandomForce.h"
#include "../interactions/CilForce.h"

#include "Cell.h"
#include <omp.h>

class TwoBodyForceVectors
{
private:
	std::vector< std::vector<TwoBodyForce* > > intraForces;
	std::vector< std::vector<TwoBodyForce* > > interForces;
public:
	TwoBodyForceVectors() {}
	TwoBodyForceVectors(size_t numberOfTypes);

	std::vector< std::vector<TwoBodyForce* > >&  getIntraForces() { return intraForces; }
	std::vector< std::vector<TwoBodyForce* > >&  getInterForces() { return interForces; }

	const std::vector< TwoBodyForce* >& operator()(const Part* part1, const Part* part2, size_t numberOfTypes) const;

};


class PartSpecs : public BinarySavable
{
private:
	typedef  std::vector< std::vector<OneBodyForce* > > OneBodyForceVector;

public:
	size_t					numberOfTypes;
	std::vector<PartType>	partTypes;
	OneBodyForceVector		oneBodyForces;
	TwoBodyForceVectors		twoBodyForces;
	
public:
	PartSpecs() : numberOfTypes(0) {}

	PartSpecs(size_t n);

	TwoBodyForce*		getTwoBodyForce(const Part* part1, const Part* part2, size_t index) const;
	size_t				numberOfTwoBodyForces(const Part* part1, const Part* part2) const;

	std::string& getName(size_t i) { return partTypes.at(i).name; };
	double getDiameter(size_t i) { return partTypes.at(i).sig; };

	bool load(Hdf5* file, const char* groupName)		  = 0;
	bool save(Hdf5* file, const char* groupName) const    = 0;

	virtual bool cellIsBroken(const Cell* cell, const Box* box) const = 0;
};

