#pragma once
#include "../Cell.h"
#include "../../box/Box.h"
#include "checkLength.h"

class CheckBroken
{
public:

	static bool tooLong(const Cell* cell, const Box* box, double threshold)
	{
		if (CheckLength::cellIsLongerThan(threshold * threshold, cell, box) )
		{
			std::cerr << "Broken cell detected." << std::endl;
			return true;
		} else {
			return false;
		}
	}

};