#include "wrappers.h"


namespace pywrapper {
	using namespace boost::python;

	BOOST_PYTHON_MODULE(Cellulae) {


		register_exception_translator<std::runtime_error>(&translate);

		class_<Vector>("Vector", init<>())
			// non default constructors
			.def(init<double, double>())
			// members
			.def_readonly("x", &Vector::x)
			.def_readonly("y", &Vector::y)
			// routines
			.def("dotProduct", &Vector::dotProduct)
			;

		class_<Hdf5>("Hdf5", init<char*, unsigned int>())
			.def("closeFile", &Hdf5::closeFile)
			;

		class_<myOpenmp>("Omp", init<int>())
			.def("activate", &myOpenmp::activate)
			;

		class_<Parameters>("Parameters", init<>())
			.def("addParam", &Parameters::addParam)
			;

		class_<CellColony>("CellColony", init<>())
			.def(init<int>())
			.def("save",				&CellColony::save)
			.def("load",				&CellColony::load)
			.def("populate",			&CellColony::populate)
			.def("populateSlab",		&CellColony::populateSlab)
			.def("addOnePartCell",		&CellColony::addOnePartCell)
			.def("addTwoPartsCell",		&CellColony::addTwoPartsCell )
			.def("size",				&CellColony::size)
			.def("reserve",				&CellColony::reserve)
			.def("totalNumberOfParts",	&CellColony::totalNumberOfParts)
			.def("getPartX",			&CellColony::getPartX)
			.def("getPartY",			&CellColony::getPartY)
			.def("assignCycleStage",	&CellColony::assignCycleStage)
			;

		class_<Box>("Box", init<>())
			.def(init<double, double, bool, int, int>())
			.def("clearSubBoxes", &Box::clearSubBoxes)
			.def("save", &Box::save)
			.def("load", &Box::load)
			;

		//class_<PartSpecs>("PartSpecs", no_init)
		//	;

		class_<OneBodyForceWrap, boost::noncopyable>("OneBodyForce")
			;

		class_<PlanarWall, bases<OneBodyForce> >("PlanarWall")
			.def(init<double, double, double, int, double>())
			.def("setEpsilon", &PlanarWall::setEpsilon)
			;

		class_<PartSpecsWrap, boost::noncopyable>("PartSpecs")
			.def("addOneBodyForce", &PartSpecs::addOneBodyForce)
			.def("load", pure_virtual(&PartSpecs::load))
			.def("save", pure_virtual(&PartSpecs::save))
			.def("cellIsBroken", pure_virtual(&PartSpecs::cellIsBroken))
			.def("cellIsDead", pure_virtual(&PartSpecs::cellIsDead))
			.def("cellDuplicates", pure_virtual(&PartSpecs::cellDuplicates))
			.def("endOfDivisionStage",	pure_virtual(&PartSpecs::endOfDivisionStage))
			.def("divisionCriterion", pure_virtual(&PartSpecs::divisionCriterion))
			.def("updateStage", pure_virtual(&PartSpecs::updateStage))
			;
/*
		class_<SMTYSpecs, bases<PartSpecs> >("SMTYSpecs", init<>())
			.def(init<double, double, double, double, double, double, double, double, double, double, double>())
			.def("save", &SMTYSpecs::save)
			.def("load", &SMTYSpecs::load)
			.def("cellIsBroken", &SMTYSpecs::cellIsBroken)
			.def("cellIsDead", &SMTYSpecs::cellIsDead)
			.def("cellDuplicates", &SMTYSpecs::cellDuplicates)
			;

		class_<SMTYSpecsCycle, bases<PartSpecs> >("SMTYSpecsCycle", init<>())
			.def(init<Parameters*, size_t, size_t>())
			.def("save",		   &SMTYSpecsCycle::save)
			.def("load",		   &SMTYSpecsCycle::load)
			.def("cellIsBroken",   &SMTYSpecsCycle::cellIsBroken)
			.def("cellIsDead",     &SMTYSpecsCycle::cellIsDead)
			.def("cellDuplicates", &SMTYSpecsCycle::cellDuplicates)
			.def("getCycleLength", &SMTYSpecsCycle::getCycleLength)
			;

		class_<SMTYSpecsCycleSoftCore, bases<PartSpecs> >("SMTYSpecsCycleSoftCore", init<>())
			.def(init<Parameters*, size_t, size_t>())
			.def("save", &SMTYSpecsCycleSoftCore::save)
			.def("load", &SMTYSpecsCycleSoftCore::load)
			.def("cellIsBroken", &SMTYSpecsCycleSoftCore::cellIsBroken)
			.def("cellIsDead",   &SMTYSpecsCycleSoftCore::cellIsDead)
			.def("cellDuplicates", &SMTYSpecsCycleSoftCore::cellDuplicates)
			.def("getCycleLength", &SMTYSpecsCycleSoftCore::getCycleLength)
			;
*/

		class_<SMTYSpecsCycleSoftCoreNOCIL, bases<PartSpecs> >("SMTYSpecsCycleSoftCoreNOCIL", init<>())
			.def(init<Parameters*, size_t, size_t>())
			.def("save", &SMTYSpecsCycleSoftCoreNOCIL::save)
			.def("load", &SMTYSpecsCycleSoftCoreNOCIL::load)
			.def("cellIsBroken", &SMTYSpecsCycleSoftCoreNOCIL::cellIsBroken)
			.def("cellIsDead", &SMTYSpecsCycleSoftCoreNOCIL::cellIsDead)
			.def("cellDuplicates", &SMTYSpecsCycleSoftCoreNOCIL::cellDuplicates)
			.def("getCycleLength", &SMTYSpecsCycleSoftCoreNOCIL::getCycleLength)
			;

/*
		class_<SMTYSpecsAdhesion, bases<PartSpecs> >("SMTYSpecsAdhesion", init<>())
			.def(init<double, double, double, double, double, double, double, double, double, double, double>())
			.def("save", &SMTYSpecsAdhesion::save)
			.def("load", &SMTYSpecsAdhesion::load)
			.def("cellIsBroken", &SMTYSpecsAdhesion::cellIsBroken)
			.def("cellIsBroken",   &SMTYSpecsAdhesion::cellIsBroken)
			.def("cellIsDead",     &SMTYSpecsAdhesion::cellIsDead)
			.def("cellDuplicates", &SMTYSpecsAdhesion::cellDuplicates)
			;

		class_<SMTYSpecsNOCIL, bases<PartSpecs> >("SMTYSpecsNOCIL", init<>())
			.def(init<double, double, double, double, double, double, double, double, double, double, double>())
			.def("save", &SMTYSpecsNOCIL::save)
			.def("load", &SMTYSpecsNOCIL::load)
			.def("cellIsBroken", &SMTYSpecs::cellIsBroken)
			.def("cellIsBroken",   &SMTYSpecsNOCIL::cellIsBroken)
			.def("cellIsDead",     &SMTYSpecsNOCIL::cellIsDead)
			.def("cellDuplicates", &SMTYSpecsNOCIL::cellDuplicates)
			;

		class_<SMTYSpecsSticky, bases<PartSpecs> >("SMTYSpecsSticky", init<>())
			.def(init<Parameters*>())
			.def("save", &SMTYSpecsSticky::save)
			.def("load", &SMTYSpecsSticky::load)
			.def("cellIsBroken", &SMTYSpecsSticky::cellIsBroken)
			.def("cellIsBroken",   &SMTYSpecsSticky::cellIsBroken)
			.def("cellIsDead",     &SMTYSpecsSticky::cellIsDead)
			.def("cellDuplicates", &SMTYSpecsSticky::cellDuplicates)
			;

		class_<MonoZimm, bases<PartSpecs> >("MonoZimm", init<>())
			.def(init<double, double, double, double, double>())
			.def("save", &MonoZimm::save)
			.def("load", &MonoZimm::load)
			.def("cellIsBroken", &MonoZimm::cellIsBroken)
			.def("cellIsBroken",   &MonoZimm::cellIsBroken)
			.def("cellIsDead",     &MonoZimm::cellIsDead)
			.def("cellDuplicates", &MonoZimm::cellDuplicates)
			;
*/


		class_<System>("System", init<CellColony*, Box*, size_t>())
			.def("addVelocity", &System::addVelocity)
			.def("updatePositions", &System::updatePositions)
			.def("computeForces", &System::computeForces)
			.def("computeForcesVoronoi", &System::computeForcesVoronoi)
			//.def("collectForces",  &System::collect)
			.def("getColony", &System::getColony)
			.def("getNumberOfCells", &System::getNumberOfCells)
			.def("cellsAreBroken", &System::cellsAreBroken)
			.def("twinCells", &System::twinCells)
			.def("eraseDeadCells", &System::eraseDeadCells)
			.def("eraseRegionCells", &System::eraseRegionCells)
			.def("duplicateCells", &System::duplicateCells)
			.def("setTypeFriction", &System::setTypeFriction)
			.def("getTypeFriction", &System::getTypeFriction)
			.def("setPartSpecs", &System::setPartSpecs)
			.def("addPartSpecs", &System::addPartSpecs)
			.def("setCycleLength", &System::setCycleLength)
			;


		class_<SystemTrajectory>( "SystemTrajectory", init<>())
			.def("save", &SystemTrajectory::save)
			.def("load", &SystemTrajectory::load)
			.def("size", &SystemTrajectory::size)
			.def("push_back", &SystemTrajectory::push_back)
			.def("copyColony", &SystemTrajectory::copyColony)
			.def("assignCycleStage", &SystemTrajectory::assignCycleStage)
			;



#ifdef VISUALS
		class_<visuals::Display, boost::noncopyable>("Display", init<int, int, double, Box*, PartSpecs*, SystemTrajectory*, bool>())
			.def("run", &visuals::Display::run)
			;
#endif

	}




}
