#pragma once
#include "PartSpecs.h"

class SMTYSpecsNOCIL : public PartSpecs
{
	// params
	double m_sigAA;
	double m_sigBB;
	double m_motility;
	double m_epsilon;
	double m_cut;
	double m_rMaxSquared;
	double m_kappa;
	double m_friction;

public:
	SMTYSpecsNOCIL();
	SMTYSpecsNOCIL(double sigAA, double sigBB, double motility,
		double epsilon, double cut, double rMaxSquared, double kappa, double friction);

	inline double getSigBB() { return m_sigBB; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;

private:
	void SMTYSpecsNOCIL::build();

};
