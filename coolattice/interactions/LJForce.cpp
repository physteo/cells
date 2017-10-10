#include "LJForce.h"


void LJForce::updateForce(Part* part1, const Part* part2, const Box* box) const
{
	Vector distance{};
	double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distance);

	if (r2 < rcut2)
	{
		double one_over_r2 = 1.0 / r2;
		double sigma2_over_r2 = sig * sig / r2;
		double sigma6_over_r6 = sigma2_over_r2 * sigma2_over_r2 * sigma2_over_r2;

		double force = 48.0 * eps * one_over_r2 * sigma6_over_r6 * (sigma6_over_r6 - 0.5);

		Vector addedForce = distance * force;

		part1->velocity += addedForce * (1.0 / (1.0)); // TODO URGENT: change to 1.0 / FRICTION
	}

}
