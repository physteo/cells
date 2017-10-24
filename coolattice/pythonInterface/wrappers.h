#pragma once


#include <iostream>
#include <string>
#include <boost/python.hpp>

#include "../definitions.h"
#include "../math/Vector.h"
#include "../particles/PartSpecs.h"
#include "../utils/myOpenmp.h"


#include "../particles/CellColony.h"
#include "../particles/SMTYSpecs.h"
#include "../particles/SMTYSpecsNOCIL.h"
#include "../particles/SMTYSpecsAdhesion.h"


#include "../system/System.h"
#include "../system/SystemTrajectory.h"

#ifdef VISUALS
#include "../visuals/Display.h"
#endif

namespace pywrapper {

	class OneBodyForceWrap : public OneBodyForce, public boost::python::wrapper<OneBodyForce>
	{
		void updateForce(Part* part, const Box* box, Vector& forceVector) const override
		{
			this->get_override("updateForce")();
		}

	};


	class PartSpecsWrap : public PartSpecs, public boost::python::wrapper<PartSpecs>
	{
		bool load(Hdf5* file, const char* groupName) override
		{
			return this->get_override("load")();
		}

		bool save(Hdf5* file, const char* groupName) const override
		{
			return this->get_override("save")();
		}

		bool cellIsBroken(const Cell* cell, const Box* box) const override
		{
			return this->get_override("cellIsBroken")();
		}


	};



}