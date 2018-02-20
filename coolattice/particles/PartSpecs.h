#pragma once

#define AT_OPTIMIZED


#include <vector>
#include <assert.h>
#include <memory>
#include <string.h> // for strcmp

#include "../math/Vector.h"
#include "PartType.h"

#include "./utilities/cycleManager.h"

#include "../interactions/Force.h"
#include "../interactions/TwoBodyForce.h"

#include "../interactions/LJForce.h"
#include "../interactions/FeneForce.h"
#include "../interactions/GaussianRandomForce.h"
#include "../interactions/CilForce.h"

#include "Cell.h"
#include <omp.h>

// \bried Contains all one-body forces. Slot i,j of the matrix corresponds to the stage i, particle type j.
typedef  std::vector< std::vector<OneBodyForce* > > OneBodyForceVector;

// \brief Class for storing all the two-body forces acting on the system.
// You don't need to use this class usually, as it is only used by the abstract class PartSpecs.
// This class contains all the TwoBodyForces acting on the system. It contains two matrices internally: intraForces and interForces.
// Each matrix's entry i,j corresponds to stage i, and j is the index which runs for each pair of types:
// j = 0 means interaction between type 0 and type 0, acting only on type 0
// j = 1 means interaction between type 0 and type 1, acting only on type 0
// j = 2 means interaction between type 1 and type 0, acting only on type 1
// j = 3 means interaction between type 1 and type 1, acting only on type 1
// For example, at stage i, the force acting on 0 between element 1 and element 0 of the same cell, is obtained using getIntraForces().at(i).at(1). But usually 
class TwoBodyForceVectors;


// \brief Abstract class for specifying the system. 
class PartSpecs : public BinarySavable
{
public:
	const std::string& getPartName(size_t stage, size_t partTypeIndex)	const { return partTypes.at(stage).getPartTypes().at(partTypeIndex).name;     }
	double getDiameter(size_t stage, size_t partTypeIndex)			const { return partTypes.at(stage).getPartTypes().at(partTypeIndex).sig;      }
	double getMass    (size_t stage, size_t partTypeIndex)			const { return partTypes.at(stage).getPartTypes().at(partTypeIndex).mass;     }
	double getFriction(size_t stage, size_t partTypeIndex)			const { return partTypes.at(stage).getPartTypes().at(partTypeIndex).friction; }
	double getDiameter(const Part* part)	const { return partTypes.at(part->currentStage).getPartTypes().at(part->type).sig; }
	double getMass	  (const Part* part)	const { return partTypes.at(part->currentStage).getPartTypes().at(part->type).mass; }
	double getFriction(const Part* part)	const { return partTypes.at(part->currentStage).getPartTypes().at(part->type).friction; }

	size_t getNumberOfStages() { return partTypes.size(); }
	size_t getNumberOfTypes() { return numberOfTypes; }
	const std::vector<PartTypeVector>& getVectorOfPartTypes() { return partTypes; }

	void addOneBodyForce(size_t stage, size_t slot, OneBodyForce* force);
	void addIntraForce  (size_t stage, size_t slot, TwoBodyForce* force);
	void addInterForce  (size_t stage, size_t slot, TwoBodyForce* force);
	
	// update the stage of the particle
	void updateStage(size_t time, Cell* cell) const;

	// computes all the one-body forces that act on a particle at its correct stage, and updates the part1's forces.
	void computeOneBodyForces(Part* part1, Box* box) const;

	// computes all the two-body forces that act on part1 caused by part2, and updates only part1's forces 
	// ( -> third principle of newton is not exployed. This is to make parallelization easier)
	void computeTwoBodyForces(Part* part1, const Part* part2, Box* box) const;
	void optimized_CG_computeTwoBodyForces(Part* part1, const Part* part2, Box* box) const;


	// checks if the cell has terminated successfully the division stage, if there is any.
	virtual bool   endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const = 0;

	// specifies how the new cells are created from the initial dividing cell. the initial cell is updated and 
	// all the new cells are inserted into the vector newCells. It needs to know the current cellCounter in order
	// to assign the proper cell ID to the new cells, and also the cycleLength to assign randomly the stage.
	virtual bool   cellDivides(Cell* cell, std::vector<Cell>* newCells,
		const Box* box, size_t& cellCounter, size_t cycleLength)  const = 0;

	// returns True if the cell is dead
	virtual bool cellIsDead(const Cell* cell, const Box* box) = 0; 

	// returns True if the cell is broken, in the sense that there is some unwanted behaviour happening
	virtual bool cellIsBroken(const Cell* cell, const Box* box) const = 0;
	
	
	// specifies how to load the partSpecs from a file
	virtual bool load(Hdf5* file, const char* groupName)		  = 0;
	// specifies how to save the partSpecs into a file
	virtual bool save(Hdf5* file, const char* groupName) const    = 0;


protected:
	std::string					name;
	size_t						numberOfTypes;
	std::vector<PartTypeVector>	partTypes;
	CycleManager*				cycleManager;

	// to be used inside derived classes to initialize the number of types and number of stages, as well as the name of the partSpecs
	void init(size_t numberOfTypesIn, size_t numberOfStagesIn, const std::string& nameIn);
	// specifies the requirements for a cell division
	virtual bool   divisionCriterion(Cell* cell, const Box* box)  const = 0;


private:
	std::vector<OneBodyForceVector>		oneBodyForces;
	std::vector<TwoBodyForceVectors>	twoBodyForces;
	
	std::array<std::vector<std::vector<size_t> >, 2>   tableNumberOfTwoBodyForces;


	void addTwoBodyForce(size_t stage, size_t slot, int intra, TwoBodyForce* force);	

	//TwoBodyForce*		test_get_TwoBodyForce(const Part* part1, const Part* part2) const;

	TwoBodyForce*		getTwoBodyForce(const Part* part1, const Part* part2, size_t index, size_t stage) const;
	size_t				numberOfTwoBodyForces(const Part* part1, const Part* part2, size_t stage) const;
};



class TwoBodyForceVectors
{

public:
	TwoBodyForceVectors() {}
	TwoBodyForceVectors(size_t numberOfTypes);

	std::vector< std::vector<TwoBodyForce* > >&  getIntraForces() { return intraForces; }
	std::vector< std::vector<TwoBodyForce* > >&  getInterForces() { return interForces; }


	const std::vector< TwoBodyForce* >& operator()(const Part* part1, const Part* part2, size_t numberOfTypes) const;

private:
	std::vector< std::vector<TwoBodyForce* > > intraForces;
	std::vector< std::vector<TwoBodyForce* > > interForces;

};