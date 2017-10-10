
#include "CilForce.h"

void CilForce::updateForce(Part* part1, const Part* part2, const Box* box) const
{
	Vector distance;
	box->computeDistanceVectorPBC(part1->position, part2->position, distance);

	part1->velocity += distance * motility;
}


void ConstantPropulsionForce::updateForce(Part* part1, const Part* part2, const Box* box) const
{
	Vector distance;
	box->computeDistanceVectorPBC(part1->position, part2->position, distance);
	double distanceLength = sqrt(Vector::dotProduct(distance, distance));
	if(distanceLength > 0.0)
		distance = distance * (1.0 / distanceLength );

	part1->velocity += distance * intensity;
}