#include "FeneForce.h"


void FeneForce::updateForce(Part* part1, const Part* part2, const Box* box) const
{
	Vector distanceVector{};
	double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distanceVector);
	part1->velocity += distanceVector * (-kappa / (1.0 - r2 / rMaxSquared));
}