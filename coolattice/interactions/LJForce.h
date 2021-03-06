#pragma once
#include "TwoBodyForce.h"
#include <cmath>

class NewLJForce : public TwoBodyForce
{
	double m_eps;
	double m_rcut2coefficient;
public:
	NewLJForce(double eps, double rcut2coefficient)
		: m_eps(eps), m_rcut2coefficient(rcut2coefficient) {}
	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};


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

class MorseForce : public TwoBodyForce
{

	double m_eps;
	double m_rho;
	double m_cut;
public:
	MorseForce(double eps, double rho, double cut) : m_eps(eps), m_rho(rho), m_cut(cut) {}
	~MorseForce() {}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
};