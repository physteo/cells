#pragma once
#include "TwoBodyForce.h"
#include <cmath>

class ZimmermannRepAdh : public TwoBodyForce
{
public:
	double A;
	double B;
	double C;

public:
	ZimmermannRepAdh(double Ain = 0.0, double Bin = 0.0, double Cin = 0.0)
		: A(Ain), B(Bin), C(Cin) {}
	~ZimmermannRepAdh() {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};
