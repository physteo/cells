#pragma once
#include "TwoBodyForce.h"

class LJForce : public TwoBodyForce
{
public:
	double sig;
	double eps;
	double rcut2;

public:
	LJForce(double sigIn = 1.0, double epsIn = 0.0, double rcut2In = 1.0) 
		: sig(sigIn), eps(epsIn), rcut2(rcut2In) {}
	~LJForce() {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};

