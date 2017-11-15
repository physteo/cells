#include "SMSoftCore.h"


void SMSoftCore::updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const
{
	Vector distance{};
	double r2 = box->computeDistanceSquaredPBC(part1->position, part2->position, distance);
	forceVector = Vector{ 0.0, 0.0 };
	if (r2 < rcut2)
	{
		double r = sqrt(r2);
		double y = r - sig;
		double force = 0.0;

		if (y <= 0)
		{
			if (y >= -xi)
			{
				force = (ewell + ecore) * t_prime(-y) / r;
			}
		}
		else if (y > 0)
		{
			if (y <= xi)
			{
				force = -ewell * t_prime(y) / r;
			}
		}

		forceVector = distance * force;
		part1->velocity += forceVector;
	}

}