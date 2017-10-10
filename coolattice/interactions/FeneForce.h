#pragma once
#include "TwoBodyForce.h"

class FeneForce : public TwoBodyForce
{
public:
	double rMaxSquared;
	double kappa;

public:
	FeneForce(double rMaxSquaredIn = 1.0, double kappaIn = 0.0): rMaxSquared(rMaxSquaredIn), kappa(kappaIn) {}
	~FeneForce() {}

	void updateForce(Part* part1, const Part* part2, const Box* box) const override;

};