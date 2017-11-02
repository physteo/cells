#pragma once

#include <cmath>

#include "TwoBodyForce.h"

class RepulsionAdhesion : public TwoBodyForce
{
public:
	double f0;
	double f1;
	double rcut2;

public:
	RepulsionAdhesion(double f0In = 0.0, double f1In = 0.0, double rcut2In = 0.0)
		: f0(f0In), f1(f1In), rcut2(rcut2In) {}
	~RepulsionAdhesion() {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};