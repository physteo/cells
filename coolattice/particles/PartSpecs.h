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

// This "class" is a vector of OneBodyForces. Slot i of the vector corresponds to the particle type i, which is specified in PartTypes
typedef  std::vector< std::vector<OneBodyForce* > > OneBodyForceVector;

// This class contains all the TwoBodyForces acting on the system. It contains two vectors internally, one for intercellular forces
// and one for intercellular forces
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

// Abstract class for specifying the system. 
class PartSpecs : public BinarySavable
{
protected:
	std::string name;

public:
	size_t								numberOfTypes;
	std::vector<PartTypeVector>         partTypes;
	std::vector<OneBodyForceVector>		oneBodyForces;
	std::vector<TwoBodyForceVectors>	twoBodyForces;
public:
	PartSpecs() : numberOfTypes(0), name("") {}

	PartSpecs(size_t n, size_t numberOfStages);

	TwoBodyForce*		getTwoBodyForce(const Part* part1, const Part* part2, size_t index, size_t stage) const;
	size_t				numberOfTwoBodyForces(const Part* part1, const Part* part2, size_t stage) const;
	
	const std::string& getName(size_t stage, size_t i)		const { return partTypes.at(stage).getPartTypes().at(i).name;    }
	double getDiameter(size_t stage, size_t i)				const { return partTypes.at(stage).getPartTypes().at(i).sig;      }
	double getMass    (size_t stage, size_t i)				const { return partTypes.at(stage).getPartTypes().at(i).mass;     }
	double getFriction(size_t stage, size_t i)				const { return partTypes.at(stage).getPartTypes().at(i).friction; }

	void addOneBodyForce(size_t stage, size_t slot, OneBodyForce* force);
	void addIntraForce  (size_t stage, size_t slot, TwoBodyForce* force);
	void addInterForce  (size_t stage, size_t slot, TwoBodyForce* force);


	virtual bool cellIsBroken(const Cell* cell, const Box* box) const = 0;
	virtual bool cellIsDead(const Cell* cell, const Box* box) = 0; 

	// new features
	virtual void computeOneBodyForces(Part* part1, Box* box) const;
	virtual void computeTwoBodyForces(Part* part1, const Part* part2, Box* box) const;

	// specifies how the new cells are created from the initial dividing cell
	virtual bool   cellDuplicates(Cell* cell, std::vector<Cell>* newCells,
		const Box* box, size_t& cellCounter, size_t cycleLength)  const = 0;
	// update the stage of the particle
	virtual void   updateStage(size_t time, Cell* cell)           const = 0;
	// checks if the cell has terminated the division stage
	virtual bool   endOfDivisionStage(Cell* cell, const Box* box) const = 0;
	// specifies the requirements for a cell division
	virtual bool   divisionCriterion(Cell* cell, const Box* box)  const = 0;


	bool load(Hdf5* file, const char* groupName)		  = 0;
	bool save(Hdf5* file, const char* groupName) const    = 0;


private:
	void addTwoBodyForce(size_t stage, size_t slot, int intra, TwoBodyForce* force);
};

