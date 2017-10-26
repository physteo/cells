#pragma once
#include "../utils/Hdf5.h"
#include "PartSpecs.h"
#include "../interactions/ZimmermannRepAdh.h"
#include "../interactions/PlanarWall.h"


// Schnyder-Molina-Tanaka-Yamamoto model for cells, with adhesion repulsion from Basan 2013 instead of LJ
class SMTYSpecsAdhesion : public PartSpecs
{
	// params
	double m_motility;
	
	//double m_sigAA;
	//double m_sigBB;
	double m_A;
	double m_C;

	double m_rMaxSquared;
	double m_kappa;

public:
	SMTYSpecsAdhesion();
	SMTYSpecsAdhesion(double sigAA, double sigBB, double A, double C, double motility,
		double rMaxSquared, double kappa, double frictionF, double frictionB, double massF, double massB);

	inline double getSigBB() { return this->partTypes.getPartTypes().at(1).sig; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;

private:
	void SMTYSpecsAdhesion::build();

};


