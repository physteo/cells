#pragma once

#include <cmath> // for sqrt
#include "TwoBodyForce.h"

class CilForce : public TwoBodyForce
{
public:
	double motility;

public:
	CilForce(double motilityIn = 0.0) : motility(motilityIn) {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};



class ConstantPropulsionForce : public TwoBodyForce
{
public:
	double intensity;

public:
	ConstantPropulsionForce(double intensityIn = 0.0) : intensity(intensityIn) {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};