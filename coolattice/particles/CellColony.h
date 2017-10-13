#pragma once

#include <iostream>

#include "../utils/Hdf5.h"
#include "Cell.h"
#include "../utils/Hdf5types.h"

// iostream
#include <iostream>

// random:
#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>

extern gsl_rng *g_rng;


class CellColony : public BinarySavable
{
private:
	std::vector<Cell> m_cells;
public:
	CellColony() : m_cells(0) {}
	void populate(double numPerLineX, double numPerLineY, double boxLengthX, double boxLengthY, double sigMax);
	void populateSlab(double N, double f, double boxLengthX, double boxLengthY, double sigMin);

	void addTwoPartsCell(double x1, double y1, double vx1, double vy1,
		double x2, double y2, double vx2, double vy2);


	// for comparison with john
	inline double getPartX(size_t cell, int t) { return m_cells.at(cell).getPart(t).position.x; }
	inline double getPartY(size_t cell, int t) { return m_cells.at(cell).getPart(t).position.y; }


	bool save(Hdf5* file, const char* name) const override;
	bool load(Hdf5* file, const char* name) override;

	inline size_t   size() const { return m_cells.size(); };
	size_t			totalNumberOfParts() const;

	void CellColony::push_back(const Cell& cc);
	Cell& CellColony::at(size_t i);
	const Cell& CellColony::at(size_t i) const;
	void CellColony::resize(size_t i);
	void reserve(size_t i);

private:
	struct LightPart {
		double x;
		double y;
		double vx;
		double vy;
	};

	struct LightPartwCell {
		double x;
		double y;
		double vx;
		double vy;
		size_t cell;
	};


};
