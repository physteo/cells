#pragma once
#include "../utils/Hdf5.h"
#include "PartSpecs.h"
#include "../interactions/Sigmoidal.h"

#include "../pythonInterface/Parameters.h"

// Schnyder-Molina-Tanaka-Yamamoto model for cells, with no noise(for the moment).
class SMTYSpecsSticky : public PartSpecs
{
	// params
	double m_motility;
	double m_rMaxSquared;
	double m_kappa;

	double m_a1;
	double m_a2;
	
	double m_b1;
	double m_b2;
	
	double m_gamma1;
	double m_gamma2;

	double m_sigma2factor;
	double m_cut;

	double m_KT;
	double m_variance;
	double m_dt;

public:
	SMTYSpecsSticky();
	SMTYSpecsSticky(const Parameters* params);

	inline double getSigBB() { return this->partTypes.getPartTypes().at(1).sig; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const override;


private:
	void build();

};


