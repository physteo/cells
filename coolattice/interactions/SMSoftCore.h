#pragma once
#include "TwoBodyForce.h"

class SMSoftCore : public TwoBodyForce
{
public:
	double ewell;
	double ecore;
	double xi;
	double sig;
	double rcut2;

public:
	SMSoftCore(double ewellIn = 0.0, double ecoreIn = 0.0, double xiIn = 0.0, double sigIn = 0.0)
		: ewell(ewellIn), ecore(ecoreIn), xi(xiIn), sig(sigIn)
	{
		rcut2 = (sig + xi) * (sig + xi);
	}
	~SMSoftCore() {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;

private:
	inline double t_prime(const double& z) const { return 6.0 * z * (xi - z) / (xi * xi * xi); }
};
