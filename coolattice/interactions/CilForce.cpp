
#include "CilForce.h"

void CilForce::updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const
{
	Vector distance;
	box->computeDistanceVectorPBC(part1->position, part2->position, distance);

	forceVector = distance * motility;
	part1->velocity += forceVector;
}


void ConstantPropulsionForce::updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const
{
	Vector distance;
	box->computeDistanceVectorPBC(part1->position, part2->position, distance);
	double distanceLength = sqrt(Vector::dotProduct(distance, distance));
	if(distanceLength > 0.0)
		distance = distance * (1.0 / distanceLength );
	forceVector = distance * intensity;
	part1->velocity += forceVector;
}