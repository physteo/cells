#pragma once

#include "ObjectSpecs.h"

class CellSpecs : public ObjectSpecs
{
public:
	struct cellType
	{
		double kPlus;
		double kMinus;
		// ... other cell specifications:
		// motile/non motile
		// cil force specs
		// ...
	};

public:
	virtual std::string& getName(size_t) {};
	virtual double getDiameter(size_t) {};
};