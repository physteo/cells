#pragma once

#include <array>
#include "../math/Vector.h"
#include "../particles/Part.h"

#include <iostream>

class BoxCell
{
public:
	Vector downLeftCorner;
	Vector upperRightCorner;

	std::array<BoxCell*, 9> neighbour;

	Part head;

public:
	BoxCell();

	void printSubCellList(int) const;
	int  getSubCellListLength() const;

	inline const BoxCell* getNeighbour(int n) const { return neighbour.at(n); }

};

