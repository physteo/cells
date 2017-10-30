#include "Sigmoidal.h"

void Sigmoidal::updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const
{
	Vector distance{};
	double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distance);

	if (r2 < rcut2)
	{
		double r = sqrt(r2);
		double b_exp_gamma_r = b * exp(gamma * (r - sig));
		
		double force = -2.0 * (-a*gamma* b_exp_gamma_r / ((1.0 + b_exp_gamma_r)*(1.0 + b_exp_gamma_r))   );
		forceVector = distance * (force / r);
		part1->velocity += forceVector;
	}
}
