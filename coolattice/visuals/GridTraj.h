#pragma once

#include "../box/BoxCell.h"
#include "../particles/CellColony.h"

class GridTraj
{
	std::vector<char> grid;
	std::vector<Vector > leftDownCorner;
	std::vector<Vector > rightUpperCorner;

public:
	GridTraj(size_t n)
	{
		grid.resize(n);
		leftDownCorner.resize(n);
		rightUpperCorner.resize(n);
	}
	void fillGrid(const std::vector<BoxCell>* subBoxes);


};