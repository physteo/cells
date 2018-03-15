#pragma once
#include "../utils/Hdf5.h"
#include "../pythonInterface/Parameters.h"
#include "PartSpecs.h"
#include "../interactions/LJForce.h"
#include "../interactions/SMSoftCore.h"


#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>
extern gsl_rng *g_rng;

#include "utilities/checkBroken.h"
#include "utilities/checkDead.h"
#include "utilities/division.h"
#include "utilities/cycleManager.h"

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>
extern gsl_rng *g_rng;


// Schnyder-Molina-Tanaka-Yamamoto model for cells, with Soft MIGRATION repulsion/attraction and no noise(for the moment).
class SpecsTopological : public PartSpecs
{
	// name
	const std::string specsName = "Specs-topological";

	// params
	double m_motility;

	double m_epsilon;
	double m_cut;

	double m_rMaxSquared;
	double m_kappa;
	double m_rateDuplication;
	double m_thresholdDuplication;
	size_t m_divisionCycleLength;
	size_t m_numberOfStages;
	bool   m_CIL;
	bool   m_division;
	double m_ssCellLength;
	double m_ssSpeed;
	double m_migrationTime;

	enum {
		MIGRATION = 0,
		DIVISION = 1
	};

public:
	SpecsTopological();
	SpecsTopological(const Parameters* params, size_t divisionCycleLength, bool CIL, bool division);

	inline double getSigBB(size_t stage) const { return this->partTypes.at(stage).getPartTypes().at(1).sig; }

	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDivides(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const override;

	bool endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const override;
	bool divisionCriterion(Cell* cell, const Box* box)const override;

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;

private:
	SpecsTopological(double sigAA, double sigBB, double motility,
		double epsilon, double cut, double rMaxSquared, double kappa,
		double frictionF, double frictionB, double massF, double massB,
		double rateDuplication, double thresholdDuplication,
		size_t divisionCycleLength, bool CIL, bool division);
	void build();
	//size_t stageCalculation(size_t time, Part* part);

};



