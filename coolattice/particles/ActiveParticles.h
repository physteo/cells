#pragma once
#include "../utils/Hdf5.h"
#include "../pythonInterface/Parameters.h"
#include "PartSpecs.h"
#include "../interactions/LJForce.h"

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>
extern gsl_rng *g_rng;

#include "utilities/checkBroken.h"
#include "utilities/checkDead.h"
#include "utilities/division.h"
#include "utilities/cycleManager.h"

// active brownian particles
class ActiveParticles : public PartSpecs
{
	// name
	const std::string specsName = "ActiveParticles";

	// params
	double m_epsilon;
	double m_motility;
	double m_cut;
	double m_temperature;
	double m_deltat;

	size_t m_numberOfStages;


public:
	ActiveParticles();
	ActiveParticles(double epsilon, double motility, double cut, double friction, double mass, double temperature, double deltat);



	inline double getSigBB(size_t stage) const { return this->partTypes.at(stage).getPartTypes().at(1).sig; }

	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDivides(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const override;

	bool endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const override;
	bool divisionCriterion(Cell* cell, const Box* box)const override;

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;

private:
	void build();
	//size_t stageCalculation(size_t time, Part* part);

};


