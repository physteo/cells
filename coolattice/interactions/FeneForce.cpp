#include "FeneForce.h"


void FeneForce::updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const
{
	Vector distanceVector{};
	double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distanceVector);

	forceVector = distanceVector * (-kappa / (1.0 - r2 / rMaxSquared));
	part1->velocity += forceVector;
}