#pragma once

#include "../Cell.h"

#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>
extern gsl_rng *g_rng;

// \brief Abstract class for managing the progression of cell cycles.
// Derived classes specify methods for choosing the next stage the cell is going to have.
class CycleManager
{
public:
	virtual void manage(size_t time, Cell* cell) = 0;
};

// \brief Manages the cycle for cells that have a simple cycle of extension-contraction with occasional division.
class ExtensionContractionDivision : public CycleManager
{
private:
	size_t m_cycleLength;
	size_t m_divisionCycleLength;
	double m_duplicationRate;
	const size_t m_numberOfStages;
	size_t m_divisionStage;

public:
	ExtensionContractionDivision(size_t cycleLength, size_t divisionCycleLength,
		double duplicationRate, size_t divisionStage) :
		m_cycleLength(cycleLength), m_divisionCycleLength(divisionCycleLength),
		m_duplicationRate(duplicationRate), m_numberOfStages(3), m_divisionStage(divisionStage)
	{}

	void manage(size_t time, Cell* cell) override
	{
		size_t currentCycleLength;

		Part* part1 = &cell->getPart(0);
		Part* part2 = &cell->getPart(1);

		if (part1->currentStage == m_divisionStage)
		{
			currentCycleLength = m_divisionCycleLength;
		}
		else
		{
			currentCycleLength = m_cycleLength;
		}

		if (part1->currentStageTime == (currentCycleLength - 1))
		{
			// update stage
			if (gsl_rng_uniform(g_rng) < m_duplicationRate)
			{
				//std::cout << "a cell is dividing in "<< part1->position.x << "," << part1->position.y << std::endl;
				part1->currentStage = m_divisionStage;
				part2->currentStage = m_divisionStage;
			}
			else
			{
				part1->currentStage = (part1->currentStage + 1) % (m_numberOfStages - 1);
				part2->currentStage = (part2->currentStage + 1) % (m_numberOfStages - 1);
			}

			part1->currentStageTime = 0;
			part2->currentStageTime = 0;
		}


	}

};

// \brief Manages the cycle for cells that have a simple cycle of extension-contraction.
class ExtensionContraction : public CycleManager
{
private:
	size_t m_cycleLength;
	const size_t m_numberOfStages;

public:
	ExtensionContraction(size_t cycleLength) :
		m_cycleLength(cycleLength), m_numberOfStages(2)
	{}

	void manage(size_t time, Cell* cell) override
	{
		const size_t currentCycleLength = m_cycleLength;

		Part* part1 = &cell->getPart(0);
		Part* part2 = &cell->getPart(1);

		if (part1->currentStageTime == (currentCycleLength - 1))
		{
			part1->currentStage = (part1->currentStage + 1) % m_numberOfStages;
			part2->currentStage = (part2->currentStage + 1) % m_numberOfStages;
			part1->currentStageTime = 0;
			part2->currentStageTime = 0;
		}


	}

};


// \brief Manages the cycle for cells that have a single stage only.
class SingleStage : public CycleManager
{
private:
	const size_t m_numberOfStages;

public:
	SingleStage() :
		m_numberOfStages(1)
	{}

	void manage(size_t time, Cell* cell) override
	{
	}

};


class SingleStageWithDivision : public CycleManager
{
private:
	size_t m_divisionCycleLength;
	double m_duplicationRate;
	const size_t m_numberOfStages;
	size_t m_divisionStage;

public:
	SingleStageWithDivision(size_t divisionCycleLength,
		double duplicationRate, size_t divisionStage) :
		m_divisionCycleLength(divisionCycleLength),
		m_duplicationRate(duplicationRate), m_numberOfStages(2), m_divisionStage(divisionStage)
	{}

	void manage(size_t time, Cell* cell) override
	{
		Part* part1 = &cell->getPart(0);
		Part* part2 = &cell->getPart(1);

		if (part1->currentStage == m_divisionStage)
		{
			if (part1->currentStageTime == (m_divisionCycleLength - 1))
			{
				part1->currentStage = 0;
				part2->currentStage = 0;
				part1->currentStageTime = 0;
				part2->currentStageTime = 0;
			}
		}
		else
		{
			// random number for going to division stage
			if (gsl_rng_uniform(g_rng) < m_duplicationRate)
			{
				part1->currentStage = m_divisionStage;
				part2->currentStage = m_divisionStage;
			}
		}
	}



};