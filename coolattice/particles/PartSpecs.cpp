#include "PartSpecs.h"


TwoBodyForceVectors::TwoBodyForceVectors(size_t numberOfTypes)
{
	intraForces.resize(numberOfTypes * numberOfTypes);
	interForces.resize(numberOfTypes * numberOfTypes);
}

const std::vector< TwoBodyForce* >& TwoBodyForceVectors::operator()(const Part* part1, const Part* part2, size_t numberOfTypes) const
{
	size_t a = part1->type;
	size_t b = part2->type;
	bool sameCell = part1->getCell() == part2->getCell();

	assert(a < numberOfTypes && b < numberOfTypes);

	if (sameCell)
		return intraForces.at(a * numberOfTypes + b);
	else
		return interForces.at(a * numberOfTypes + b);
}

void PartSpecs::init(size_t numberOfTypesIn, size_t numberOfStagesIn, const std::string& nameIn)
{
	numberOfTypes = numberOfTypesIn;
	partTypes.reserve(numberOfStagesIn);
	twoBodyForces.reserve(numberOfStagesIn);
	oneBodyForces.reserve(numberOfStagesIn);

	for (size_t i = 0; i < numberOfStagesIn; i++)
	{
		partTypes.push_back(PartTypeVector());
		partTypes.back().getPartTypes().resize(numberOfTypes);

		oneBodyForces.push_back(OneBodyForceVector());
		oneBodyForces.back().resize(numberOfTypesIn);

		twoBodyForces.push_back(TwoBodyForceVectors());
		twoBodyForces.back() = TwoBodyForceVectors{ numberOfTypesIn };
	}
	name = nameIn;

}


TwoBodyForce* PartSpecs::getTwoBodyForce(const Part* part1, const Part* part2, size_t index, size_t stage) const
{
	const  std::vector< TwoBodyForce* >& forces = twoBodyForces.at(stage)(part1, part2, numberOfTypes);
	return forces.at(index);
}

size_t PartSpecs::numberOfTwoBodyForces(const Part* part1, const Part* part2, size_t stage) const
{
	const  std::vector< TwoBodyForce* >& forces = twoBodyForces.at(stage)(part1, part2, numberOfTypes);
	return forces.size();
}
	

void PartSpecs::addOneBodyForce(size_t stage, size_t slot, OneBodyForce* force)
{
	OneBodyForceVector& forces = this->oneBodyForces.at(stage);
	forces.at(slot).push_back(force);
}

void PartSpecs::addIntraForce(size_t stage, size_t slot, TwoBodyForce* force)
{
	addTwoBodyForce(stage, slot, 1, force);
}


void PartSpecs::addInterForce(size_t stage, size_t slot, TwoBodyForce* force)
{
	addTwoBodyForce(stage, slot, 0, force);
}

void PartSpecs::updateStage(size_t time, Cell* cell) const
{
	cycleManager->manage(time, cell);
}

void PartSpecs::addTwoBodyForce(size_t stage, size_t slot, int intra, TwoBodyForce* force)
{
	if (intra == 0)
	{
		std::vector< std::vector<TwoBodyForce* > >& forces = this->twoBodyForces.at(stage).getInterForces();
		forces.at(slot).push_back(force);
	}
	else if (intra == 1)
	{
		std::vector< std::vector<TwoBodyForce* > >& forces = this->twoBodyForces.at(stage).getIntraForces();
		forces.at(slot).push_back(force);
	}
	else {
		std::cerr << "TwoBodyForce must be either inter or intra - cellular." << std::endl;
		exit(EXIT_FAILURE);
	}

}

void PartSpecs::computeOneBodyForces(Part* part1, Box* box) const
{
	size_t stage = part1->currentStage;
	for (size_t f = 0; f < this->oneBodyForces.at(stage).at(part1->type).size(); f++)
	{
		const OneBodyForce* force = this->oneBodyForces.at(stage).at(part1->type).at(f); // .get() <- not here anymore cause i swtiched to raw pointers in order to make python interface
		Vector addedForce;
		force->updateForce(part1, box, addedForce);
	}
}


void PartSpecs::computeTwoBodyForces(Part* part1, const Part* part2, Box* box) const
{
	size_t stage = part1->currentStage;
	size_t numberOfTwoBodyForces = this->numberOfTwoBodyForces(part1, part2, stage);

	for (size_t f = 0; f < numberOfTwoBodyForces; f++)
	{
		const TwoBodyForce* force = this->getTwoBodyForce(part1, part2, f, stage);
		Vector addedForce;
		force->updateForce(part1, part2, box, addedForce);

	}
}