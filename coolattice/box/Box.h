#pragma once

#include <vector>
#include "../math/Vector.h"
#include "BoxCell.h"

#include "../definitions.h"
#include "../utils/Hdf5.h"

#include "../particles/CellColony.h"

class Box : public BinarySavable
{
public:
	Box();
	Box(double lx, double ly, bool periodicBoundaries, int subBoxesX, int subBoxesY);
	
	virtual double computeDistanceSquaredPBC(const Vector& v1, const Vector& v2, Vector& output) const;
	virtual void   computeDistanceVectorPBC(const Vector& v1, const Vector& v2, Vector& output) const;

	inline double getLx() const { return m_lx; }
	inline double getLy() const { return m_ly; }
	inline const std::vector<BoxCell>& getBoxCells() const { return m_subBoxes; };
	inline       std::vector<BoxCell>& getBoxCells()       { return m_subBoxes; };
	inline bool   isPeriodic() const { return m_periodicBoundaries; }

	inline int    getBoxCellIndex(int x, int y) const { return x + y * m_numberOfBoxCellsX; }
	inline double getBoxCellLengthX() const { return m_subBoxesLengthX; }
	inline double getBoxCellLengthY() const { return m_subBoxesLengthY; }
	inline double getNumberOfBoxCellsX() const { return m_numberOfBoxCellsX; }
	inline double getNumberOfBoxCellsY() const { return m_numberOfBoxCellsY; }

	void clearSubBoxes();
	void putPartInSubBox(Part* part);
	void putAllPartsInSubBox(CellColony* cells);

	void remap(Vector& position);

	inline int wrapIndex(int i, int nc) const { return i < 0 ? nc - 1 : ( i >= nc ? 0: i); }
	inline int wrapXIndex(int i) const { return wrapIndex(i, m_numberOfBoxCellsX); }
	inline int wrapYIndex(int i) const { return wrapIndex(i, m_numberOfBoxCellsY); }


	bool load(Hdf5* file, const char* groupName) override;
	bool save(Hdf5* file, const char* groupName) const override;

private:
	void createCubic();


private:
	double m_lx;
	double m_ly;
	bool   m_periodicBoundaries;

	// number of subBoxes and 
	int    m_numberOfBoxCellsX;
	int    m_numberOfBoxCellsY;
	double m_subBoxesLengthX;
	double m_subBoxesLengthY;

	std::vector<BoxCell> m_subBoxes;

};

