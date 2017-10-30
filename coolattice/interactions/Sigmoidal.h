#pragma once
#include <cmath>
#include "TwoBodyForce.h"

class Sigmoidal : public TwoBodyForce
{
public:
	double a;
	double b;
	double gamma;
	double sig;
	double rcut2;
public:
	Sigmoidal(double aIn = 0.0, double bIn = 0.0,
		double gammaIn=0.0, double sigIn=0.0, double rcut2In=0.0)
		: a(aIn), b(bIn), gamma(gammaIn), sig(sigIn), rcut2(rcut2In) {}
	~Sigmoidal() {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};