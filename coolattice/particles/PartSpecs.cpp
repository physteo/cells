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

	if (sameCell) {
#ifdef AT_OPTIMIZED
		return intraForces[a * numberOfTypes + b];
#else
		return intraForces.at(a * numberOfTypes + b);
#endif
	}
	else
	{
#ifdef AT_OPTIMIZED
		return interForces[a * numberOfTypes + b];
#else
		return interForces.at(a * numberOfTypes + b);
#endif
	}
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
#ifdef AT_OPTIMIZED
	const  std::vector< TwoBodyForce* >& forces = twoBodyForces[stage](part1, part2, numberOfTypes);
	return forces[index];
#else
	const  std::vector< TwoBodyForce* >& forces = twoBodyForces.at(stage)(part1, part2, numberOfTypes);
	return forces.at(index);
#endif

}

size_t PartSpecs::numberOfTwoBodyForces(const Part* part1, const Part* part2, size_t stage) const
{
	// TODO: URGENT: changed this 
	//const  std::vector< TwoBodyForce* >& forces = twoBodyForces.at(stage)(part1, part2, numberOfTypes);
	//return forces.size();

#ifdef AT_OPTIMIZED
	return this->tableNumberOfTwoBodyForces[part1->cell == part2->cell][stage][part1->type * numberOfTypes + part2->type];
#else
	return this->tableNumberOfTwoBodyForces.at(part1->cell == part2->cell).at(stage).at(part1->type * numberOfTypes + part2->type);
#endif
}
	

void PartSpecs::addOneBodyForce(size_t stage, size_t slot, OneBodyForce* force)
{
	OneBodyForceVector& forces = this->oneBodyForces.at(stage);
	forces.at(slot).push_back(force);
}

void PartSpecs::addIntraForce(size_t stage, size_t slot, TwoBodyForce* force)
{
	addTwoBodyForce(stage, slot, 1, force);
	
	while (tableNumberOfTwoBodyForces.at(1).size() <= stage)
	{
		tableNumberOfTwoBodyForces.at(1).push_back({});
	}
	
	while (tableNumberOfTwoBodyForces.at(1).at(stage).size() <= slot)
	{
		tableNumberOfTwoBodyForces.at(1).at(stage).push_back(0);
	}

	this->tableNumberOfTwoBodyForces.at(1).at(stage).at(slot)++;

}


void PartSpecs::addInterForce(size_t stage, size_t slot, TwoBodyForce* force)
{
	addTwoBodyForce(stage, slot, 0, force);
	
	while (tableNumberOfTwoBodyForces.at(0).size() <= stage)
	{
		tableNumberOfTwoBodyForces.at(0).push_back({});
	}

	while (tableNumberOfTwoBodyForces.at(0).at(stage).size() <= slot)
	{
		tableNumberOfTwoBodyForces.at(0).at(stage).push_back(0);
	}

	this->tableNumberOfTwoBodyForces.at(0).at(stage).at(slot)++;
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
#ifdef AT_OPTIMIZED
	for (size_t f = 0; f < this->oneBodyForces[stage][part1->type].size(); f++)
#else
	for (size_t f = 0; f < this->oneBodyForces.at(stage).at(part1->type).size(); f++)
#endif
	{
#ifdef AT_OPTIMIZED
		const OneBodyForce* force = this->oneBodyForces[stage][part1->type][f]; // .get() <- not here anymore cause i swtiched to raw pointers in order to make python interface
#else
		const OneBodyForce* force = this->oneBodyForces.at(stage).at(part1->type).at(f); // .get() <- not here anymore cause i swtiched to raw pointers in order to make python interface
#endif
		Vector addedForce;
		force->updateForce(part1, box, addedForce);
	}
}


size_t simpleCheck(const Part* part1, const Part* part2) 
{
	if (part1->cell == part2->cell) {
		if (part1->type == 0) {
			return 2;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}
}


void PartSpecs::optimized_CG_computeTwoBodyForces(Part* part1, const Part* part2, Box* box) const
{
	if (part1->cell == part2->cell) {
		if (part1->type == 0) {
			// two forces: FENE and Migration
			// FENE force
			Vector distanceVector{};
			double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distanceVector);

			Vector forceVector = distanceVector * (-0.482416 / (1.0 - r2 / 4.0));
			part1->velocity += forceVector;

			// Migration
			Vector distance;
			box->computeDistanceVectorPBC(part1->position, part2->position, distance);
			forceVector = distance * 1.00;
			part1->velocity += forceVector;
		}
		else {
			// FENE force
			Vector distanceVector{};
			double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distanceVector);

			Vector forceVector = distanceVector * (-0.482416 / (1.0 - r2 / 4.0));
			part1->velocity += forceVector;
		}
	}
	else {
		// Lennard Jones interaction
		Vector distance{};
		double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distance);

		Vector forceVector = Vector{ 0.0, 0.0 };

		double sig = 0.5 * (part1->currentSigma + part2->currentSigma);

		if (r2 < 1.122462048309373 * 1.122462048309373 * sig * sig)
		{
			double one_over_r2 = 1.0 / r2;
			double sigma2_over_r2 = sig * sig / r2;
			double sigma6_over_r6 = sigma2_over_r2 * sigma2_over_r2 * sigma2_over_r2;

			double force = 48.0 * 9.64832e-5 * one_over_r2 * sigma6_over_r6 * (sigma6_over_r6 - 0.5);

			forceVector = distance * force;
			part1->velocity += forceVector;
		}


	}
}



void PartSpecs::computeTwoBodyForces(Part* part1, const Part* part2, Box* box) const
{	

	size_t stage = part1->currentStage;
	//size_t numberOfTwoBodyForces = simpleCheck(part1, part2);
	size_t numberOfTwoBodyForces = this->numberOfTwoBodyForces(part1, part2, stage);
	//size_t numberOfTwoBodyForces = this->tableNumberOfTwoBodyForces.at(!(part1->cell == part2->cell)).at(stage).at(part1->type + 2 * part2->type);
	

	for (size_t f = 0; f < numberOfTwoBodyForces; f++)
	{
		const TwoBodyForce* force = this->getTwoBodyForce(part1, part2, f, stage);
		Vector addedForce;
		force->updateForce(part1, part2, box, addedForce);
	}
}