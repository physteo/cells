#pragma once


#include <iostream>
#include <string>
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/exception_translator.hpp>
#include <exception>

#include "../definitions.h"
#include "../math/Vector.h"
#include "../particles/PartSpecs.h"
#include "../utils/myOpenmp.h"

#include "../interactions/PlanarWall.h"

#include "../particles/CellColony.h"
//#include "../particles/SMTYSpecs.h"
//#include "../particles/SMTYSpecsNOCIL.h"
//#include "../particles/SMTYSpecsAdhesion.h"
//#include "../particles/SMTYSpecsSticky.h"
//#include "../particles/SMTYSpecsCycle.h"
//#include "../particles/SMTYSpecsCycleSoftCore.h"
#include "../particles/SMTYSpecsCycleSoftCore.h"
#include "../particles/SMTYSpecsCycleLJ.h"
#include "../particles/SMTYSpecsCoarseGrained.h"
#include "../particles/SMTYSpecsCoarseGrainedImproved.h"
#include "../particles/SMTYSpecsCoarseGrainedImprovedMorse.h"
#include "../particles/SpecsTopological.h"

//#include "../particles/MonoZimm.h"

#include "../system/System.h"
#include "../system/SystemTrajectory.h"


#ifdef VISUALS
#include "../visuals/Display.h"
#endif

// parameters
#include "Parameters.h"

namespace pywrapper{

	void translate(std::runtime_error const& e)
	{
		// Use the Python 'C' API to set up an exception object
		PyErr_SetString(PyExc_RuntimeError, e.what());
	}



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

		bool cellIsDead(const Cell* cell, const Box* box) override
		{
			return this->get_override("cellIsDead")();
		}

		bool cellDivides(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const override
		{
			return this->get_override("cellDivides")();
		}

		bool endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const
		{
			return this->get_override("endOfSuccessfullDivisionStage")();
		}

		bool divisionCriterion(Cell* cell, const Box* box) const
		{
			return this->get_override("divisionCriterion")();
		}

	};



}
