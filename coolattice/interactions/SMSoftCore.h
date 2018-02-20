#pragma once
#include "TwoBodyForce.h"
#include <cmath>

class NewStickyAttraction : public TwoBodyForce
{
	double m_xi;
	double m_rcut2coefficient;
	double m_ewell;

public:
	NewStickyAttraction(double xiIn = 0.0, double ewellIn = 0.0, double rcut2coefficient = 0.0)
		: m_xi(xiIn), m_ewell(ewellIn), m_rcut2coefficient(rcut2coefficient)
	{
	}
	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;
	inline double t_prime(const double& z) const { return 6.0 * z * (m_xi - z) / (m_xi * m_xi * m_xi); }
};


class NewSMSoftCore : public TwoBodyForce
{
	double m_ewell;
	double m_ecore;
	double m_xi;
	double m_rcut2coefficient;

public:
	NewSMSoftCore(double ewellIn = 0.0, double ecoreIn = 0.0, double xiIn = 0.0, double rcut2coefficient = 0.0)
		: m_ewell(ewellIn), m_ecore(ecoreIn), m_xi(xiIn), m_rcut2coefficient(rcut2coefficient)
	{
	}

	void updateForce(Part* part1, const Part* part2, const Box* box, Vector& forceVector) const override;

	inline double t_prime(const double& z) const { return 6.0 * z * (m_xi - z) / (m_xi * m_xi * m_xi); }

};



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
