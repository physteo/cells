#pragma once
#include "../utils/Hdf5.h"
#include "../pythonInterface/Parameters.h"
#include "PartSpecs.h"
#include "../interactions/SMSoftCore.h"

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>
extern gsl_rng *g_rng;



// Schnyder-Molina-Tanaka-Yamamoto model for cells, with Soft Core repulsion/attraction and no noise(for the moment).
class SMTYSpecsCycleSoftCore : public PartSpecs
{
	// params
	double m_motility;

	double m_ewell;
	double m_ecore;
	double m_xi;

	double m_rMaxSquared;
	double m_kappa;
	double m_rateDuplication;
	double m_thresholdDuplication;
	size_t m_cycleStage;
	size_t m_cycleLength;

public:
	SMTYSpecsCycleSoftCore();
	SMTYSpecsCycleSoftCore(const Parameters* params, size_t cycleStage, size_t cycleLength);
	SMTYSpecsCycleSoftCore::SMTYSpecsCycleSoftCore(double sigAA, double sigBB, double motility,
		double ewell, double ecore, double xi, double rMaxSquared, double kappa, double frictionF,
		double frictionB, double massF, double massB,
		double rateDuplication, double thresholdDuplication,
		size_t cycleStage, size_t cycleLength);

	inline double getSigBB() const { return this->partTypes.getPartTypes().at(1).sig; }
	inline size_t getCycleLength() const { return this->m_cycleLength; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t currentNumberOfCells) const override;


private:
	void build();

};

