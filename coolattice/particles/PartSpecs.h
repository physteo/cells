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

typedef  std::vector< std::vector<OneBodyForce* > > OneBodyForceVector;

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
protected:
	std::string name;
public:
	size_t					numberOfTypes;
	PartTypeVector          partTypes;
	OneBodyForceVector		oneBodyForces;
	TwoBodyForceVectors		twoBodyForces;
public:
	PartSpecs() : numberOfTypes(0), name("") {}

	PartSpecs(size_t n);

	TwoBodyForce*		getTwoBodyForce(const Part* part1, const Part* part2, size_t index) const;
	size_t				numberOfTwoBodyForces(const Part* part1, const Part* part2) const;

	const std::string& getName(size_t i) const { return partTypes.getPartTypes().at(i).name; };
	double getDiameter(size_t i)   const { return partTypes.getPartTypes().at(i).sig; };
	double getMass(size_t i)       const { return partTypes.getPartTypes().at(i).mass; }
	double getFriction(size_t i)   const { return partTypes.getPartTypes().at(i).friction; }

	void setDiameter(size_t i, double sig) { partTypes.getPartTypes().at(i).sig = sig; };
	void setMass(size_t i, double mass) {  partTypes.getPartTypes().at(i).mass = mass; }
	void setFriction(size_t i, double friction) {  partTypes.getPartTypes().at(i).friction = friction; }




	void addOneBodyForce(size_t slot, OneBodyForce* force);
	void addIntraForce(size_t slot, TwoBodyForce* force);
	void addInterForce(size_t slot, TwoBodyForce* force);


	bool load(Hdf5* file, const char* groupName)		  = 0;
	bool save(Hdf5* file, const char* groupName) const    = 0;

	virtual bool cellIsBroken(const Cell* cell, const Box* box) const = 0;
	virtual bool cellIsDead(const Cell* cell, const Box* box) const { return false; } // TODO: URGENT make pure virtual
	virtual bool cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box) const { return false; } // TODO: URGENT make pure virtual

	//virtual bool cellCanDuplicate(const Cell* cell, const Box* box) const = 0;

private:
	void addTwoBodyForce(size_t slot, int intra, TwoBodyForce* force);
};

