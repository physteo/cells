#pragma once

#include "../Cell.h"
#include "../../box/Box.h"
#include "./checkLength.h"


class CheckDead
{
public:
	static bool cellIsPulledApart(const Cell* cell, const Box* box, double thresholdSquared, size_t divisionStage)
	{
		if (CheckLength::cellIsLongerThan(thresholdSquared, cell, box)  && cell->getPart(0).currentStage != divisionStage)
		{
			return true;
		}
		else {
			return false;
		}
	}



};