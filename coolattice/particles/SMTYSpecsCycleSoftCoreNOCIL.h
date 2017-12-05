#pragma once
#include "../utils/Hdf5.h"
#include "../pythonInterface/Parameters.h"
#include "PartSpecs.h"
#include "../interactions/SMSoftCore.h"

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>
extern gsl_rng *g_rng;



// Schnyder-Molina-Tanaka-Yamamoto model for cells, with Soft Core repulsion/attraction and no noise(for the moment).
class SMTYSpecsCycleSoftCoreNOCIL : public PartSpecs
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
	size_t m_cycleLength;
	size_t m_divisionCycleLength;
	size_t m_numberOfStages;

	double m_ssCellLength;
	double m_ssSpeed;
	double m_migrationTime;

	enum {
		EXTENSION   = 0,
		CONTRACTION = 1,
		DIVISION    = 2
	};

public:
	SMTYSpecsCycleSoftCoreNOCIL();
	SMTYSpecsCycleSoftCoreNOCIL(const Parameters* params, size_t cycleLength, size_t divisionCycleLength);
	SMTYSpecsCycleSoftCoreNOCIL(double sigAA, double sigBB, double motility,
		double ewell, double ecore, double xi, double rMaxSquared, double kappa, double frictionF, double originalFrictionF,
		double frictionB, double originalFrictionB, double massF, double massB,
		double rateDuplication, double thresholdDuplication,
		size_t cycleLength, size_t divisionCycleLength);

	inline double getSigBB(size_t stage) const { return this->partTypes.at(stage).getPartTypes().at(1).sig; }
	inline size_t getCycleLength() const { return this->m_cycleLength; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;

	
	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const override;

	inline size_t getNumberOfStages() { return m_numberOfStages; }

	// new features
	bool endOfDivisionStage(Cell* cell, const Box* box) const override;
	void updateStage(size_t time, Cell* cell) const override;

	bool divisionCriterion(Cell* cell, const Box* box)const override;

private:
	void build();
	size_t stageCalculation(size_t time, Part* part);

};

