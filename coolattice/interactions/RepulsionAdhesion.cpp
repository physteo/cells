#include "RepulsionAdhesion.h"

void RepulsionAdhesion::updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const
{
	Vector distance{};
	double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distance);

	if (r2 < rcut2)
	{
		double inverse_r = 1.0 / sqrt(r2);		
		double force = -(f0 * (inverse_r - 1.0) + f1);
		forceVector = distance * (-force * inverse_r);	
		part1->velocity += forceVector;
	}
}
