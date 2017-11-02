#pragma once
#include "../utils/Hdf5.h"
#include "PartSpecs.h"
#include "../interactions/ZimmermannRepAdh.h"
#include "../interactions/PlanarWall.h"


// Single part cells which interact with repulsion adhesion from zimmermann
class MonoZimm : public PartSpecs
{
	double m_A;
	double m_C;

public:
	MonoZimm();
	MonoZimm(double sig, double A, double C, double friction, double mass);

	inline double getSigBB() { return this->partTypes.getPartTypes().at(1).sig; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t currentNumberOfCells) const override;

private:
	void build();

};

