#pragma once
#include "../utils/Hdf5.h"
#include "PartSpecs.h"
#include "../interactions/RepulsionAdhesion.h"
#include "../interactions/PlanarWall.h"


// Schnyder-Molina-Tanaka-Yamamoto model for cells, with adhesion repulsion from Basan 2013 instead of LJ
class SMTYSpecsAdhesion : public PartSpecs
{
	// params
	double m_motility;
	//double f0;
	double m_f1;
	double m_cut;
	double m_rMaxSquared;
	double m_kappa;

public:
	SMTYSpecsAdhesion();
	SMTYSpecsAdhesion(double f0, double f1, double motility,
		double cut, double rMaxSquared, double kappa, double frictionF, double frictionB, double massF, double massB);

	inline double getSigBB() { return this->partTypes.getPartTypes().at(1).sig; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;

private:
	void SMTYSpecsAdhesion::build();

};


