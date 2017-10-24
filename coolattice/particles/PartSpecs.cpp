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
	bool sameCell = part1->cell == part2->cell;

	assert(a < numberOfTypes && b < numberOfTypes);

	if (sameCell)
		return intraForces.at(a * numberOfTypes + b);
	else
		return interForces.at(a * numberOfTypes + b);
}



PartSpecs::PartSpecs(size_t n)
{
	numberOfTypes = n;
	partTypes.getPartTypes().resize(n);
	twoBodyForces = TwoBodyForceVectors{ n };
	oneBodyForces.resize(n);
	name = "";
}

TwoBodyForce* PartSpecs::getTwoBodyForce(const Part* part1, const Part* part2, size_t index) const
{
	const  std::vector< TwoBodyForce* >& forces = twoBodyForces(part1, part2, numberOfTypes);
	return forces.at(index);
}

size_t PartSpecs::numberOfTwoBodyForces(const Part* part1, const Part* part2) const
{
	const  std::vector< TwoBodyForce* >& forces = twoBodyForces(part1, part2, numberOfTypes);
	return forces.size();
}
	

void PartSpecs::addOneBodyForce(size_t slot, OneBodyForce* force)
{
	OneBodyForceVector& forces = this->oneBodyForces;
	forces.at(slot).push_back(force);
}

void PartSpecs::addIntraForce(size_t slot, TwoBodyForce* force)
{
	addTwoBodyForce(slot, 1, force);
}


void PartSpecs::addInterForce(size_t slot, TwoBodyForce* force)
{
	addTwoBodyForce(slot, 0, force);
}


void PartSpecs::addTwoBodyForce(size_t slot, int intra, TwoBodyForce* force)
{
	if (intra == 1)
	{
		std::vector< std::vector<TwoBodyForce* > >& forces = this->twoBodyForces.getIntraForces();
		forces.at(slot).push_back(force);
	}
	else if (intra == 0)
	{
		std::vector< std::vector<TwoBodyForce* > >& forces = this->twoBodyForces.getInterForces();
		forces.at(slot).push_back(force);
	}
	else {
		std::cerr << "TwoBodyForce must be either inter or intra - cellular." << std::endl;
		exit(EXIT_FAILURE);
	}

}