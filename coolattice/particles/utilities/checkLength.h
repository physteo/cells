#pragma once

#include "../Cell.h"
#include "../../box/Box.h"


class CheckLength
{
public:
	static bool cellIsLongerThan(double thresholdSquared, const Cell* cell, const Box* box)
	{
		Vector vectorDistance;
		double distance2 = box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, vectorDistance);
		if (distance2 >= thresholdSquared)
		{
			//std::cout << "cell " << cell->getPart(0).cell << ", success: " << distance2 << std::endl;
			return true;
		}
		else
		{
			//std::cout << "cell " << cell->getPart(0).cell << ", fail   : " << distance2 << std::endl;
			return false;
		}
	}

};