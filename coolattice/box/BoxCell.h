#pragma once

#include <array>
#include "../math/Vector.h"
#include "../particles/Part.h"

#include <iostream>

struct BoxCell
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

};

