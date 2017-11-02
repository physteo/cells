#pragma once
#include "PartSpecs.h"

class SMTYSpecsNOCIL : public PartSpecs
{
	// params
	//double m_sigAA;
	//double m_sigBB;
	double m_motility;
	double m_epsilon;
	double m_cut;
	double m_rMaxSquared;
	double m_kappa;
	//double m_frictionF;
	//double m_frictionB;
	//double m_massF;
	//double m_massB;


public:
	SMTYSpecsNOCIL();
	SMTYSpecsNOCIL(double sigAA, double sigBB, double motility,
		double epsilon, double cut, double rMaxSquared, double kappa, double frictionF, double frictionB, double massF, double massB);

	inline double getSigBB() { return this->partTypes.getPartTypes().at(1).sig; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t currentNumberOfCells) const override;

private:
	void build();

};
