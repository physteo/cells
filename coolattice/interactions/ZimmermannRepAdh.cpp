#include "ZimmermannRepAdh.h"

void ZimmermannRepAdh::updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const
{
	Vector distance{};
	double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distance);

	if (r2 < B)
	{
		double r = sqrt(r2);
		double force = -A * (B - r) + C * (B - r) * (B - r) * (B - r);
		forceVector = distance * (-force / r);
		part1->velocity += forceVector;
	}
}
