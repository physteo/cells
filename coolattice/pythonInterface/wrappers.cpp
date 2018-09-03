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
			.def("readName", &Hdf5::readName)
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
			.def("populateDirected",    &CellColony::populateDirected)
			.def("populateLane",        &CellColony::populateLane)
			.def("populateSlab",		&CellColony::populateSlab)
			.def("addOnePartCell",		&CellColony::addOnePartCell)
			.def("addTwoPartsCell",		&CellColony::addTwoPartsCell )
			.def("size",				&CellColony::size)
			.def("reserve",				&CellColony::reserve)
			.def("totalNumberOfParts",	&CellColony::totalNumberOfParts)
			.def("getPartX",			&CellColony::getPartX)
			.def("getPartY",			&CellColony::getPartY)
			.def("assignSingleCellStageAndTime", &CellColony::assignSingleCellStageAndTime)
			.def("assignCycleStage",	&CellColony::assignCycleStageTime)
			.def("setPolydispersity",   &CellColony::setPolydispersity)
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
			.def("getNumberOfStages", &PartSpecs::getNumberOfStages)
			.def("load", pure_virtual(&PartSpecs::load))
			.def("save", pure_virtual(&PartSpecs::save))
			.def("cellIsBroken", pure_virtual(&PartSpecs::cellIsBroken))
			.def("cellIsDead", pure_virtual(&PartSpecs::cellIsDead))
			.def("cellDivides", pure_virtual(&PartSpecs::cellDivides))
			.def("endOfSuccessfullDivisionStage",	pure_virtual(&PartSpecs::endOfSuccessfullDivisionStage))
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

		class_<SMTYSpecsCycleLJ, bases<PartSpecs> >("SMTYSpecsCycleLJ", init<>())
			.def(init<Parameters*, size_t, size_t, size_t, bool>())
			.def("save", &SMTYSpecsCycleLJ::save)
			.def("load", &SMTYSpecsCycleLJ::load)
			.def("cellIsBroken",   &SMTYSpecsCycleLJ::cellIsBroken)
			.def("cellIsDead",     &SMTYSpecsCycleLJ::cellIsDead)
			.def("cellDivides",    &SMTYSpecsCycleLJ::cellDivides)
			.def("getCycleLength", &SMTYSpecsCycleLJ::getCycleLength)
			;

		class_<SMTYSpecsCycleSoftCore, bases<PartSpecs> >("SMTYSpecsCycleSoftCore", init<>())
			.def(init<Parameters*, size_t, size_t, size_t, bool>())
			.def("save", &SMTYSpecsCycleSoftCore::save)
			.def("load", &SMTYSpecsCycleSoftCore::load)
			.def("cellIsBroken", &SMTYSpecsCycleSoftCore::cellIsBroken)
			.def("cellIsDead", &SMTYSpecsCycleSoftCore::cellIsDead)
			.def("cellDivides", &SMTYSpecsCycleSoftCore::cellDivides)
			.def("getCycleLength", &SMTYSpecsCycleSoftCore::getCycleLength)
			;

		class_<SMTYSpecsCoarseGrained, bases<PartSpecs> >("SMTYSpecsCoarseGrained", init<>())
			.def(init<Parameters*, size_t, size_t, bool>())
			.def("save",			&SMTYSpecsCoarseGrained::save)
			.def("load",			&SMTYSpecsCoarseGrained::load)
			.def("cellIsBroken",	&SMTYSpecsCoarseGrained::cellIsBroken)
			.def("cellIsDead",		&SMTYSpecsCoarseGrained::cellIsDead)
			.def("cellDivides",		&SMTYSpecsCoarseGrained::cellDivides)
			;

		class_<SMTYSpecsCoarseGrainedImproved, bases<PartSpecs> >("SMTYSpecsCoarseGrainedImproved", init<>())
			.def(init<Parameters*, size_t, size_t, bool>())
			.def("save", &SMTYSpecsCoarseGrainedImproved::save)
			.def("load", &SMTYSpecsCoarseGrainedImproved::load)
			.def("cellIsBroken", &SMTYSpecsCoarseGrainedImproved::cellIsBroken)
			.def("cellIsDead", &SMTYSpecsCoarseGrainedImproved::cellIsDead)
			.def("cellDivides", &SMTYSpecsCoarseGrainedImproved::cellDivides)
			;
		
		class_<SMTYSpecsCoarseGrainedImprovedMorse, bases<PartSpecs> >("SMTYSpecsCoarseGrainedImprovedMorse", init<>())
			.def(init<Parameters*, size_t, size_t, bool>())
			.def("save", &SMTYSpecsCoarseGrainedImprovedMorse::save)
			.def("load", &SMTYSpecsCoarseGrainedImprovedMorse::load)
			.def("cellIsBroken", &SMTYSpecsCoarseGrainedImprovedMorse::cellIsBroken)
			.def("cellIsDead", &SMTYSpecsCoarseGrainedImprovedMorse::cellIsDead)
			.def("cellDivides", &SMTYSpecsCoarseGrainedImprovedMorse::cellDivides)
			;

		class_<SpecsTopological, bases<PartSpecs> >("SpecsTopological", init<>())
			.def(init<Parameters*, size_t, size_t, bool>())
			.def("save", &SpecsTopological::save)
			.def("load", &SpecsTopological::load)
			.def("cellIsBroken", &SpecsTopological::cellIsBroken)
			.def("cellIsDead",   &SpecsTopological::cellIsDead)
			.def("cellDivides",  &SpecsTopological::cellDivides)
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
			.def("updateStages", &System::updateStages)
			.def("updatePositions", &System::updatePositions)
			.def("computeForces", &System::computeForces)
			//.def("computeForcesVoronoi", &System::computeForcesVoronoi)
			//.def("collectForces",  &System::collect)
			.def("getColony", &System::getColony)
			.def("getNumberOfCells", &System::getNumberOfCells)
			.def("cellsAreBroken", &System::cellsAreBroken)
			.def("twinCells", &System::twinCells)
			.def("eraseDeadCells", &System::eraseDeadCells)
			.def("eraseRegionCells", &System::eraseRegionCells)
			.def("divideCells", &System::divideCells)
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
			//.def("assignCycleStage", &SystemTrajectory::assignCycleStageTime) // TODO: change the name of the interfaced method to assignCycleStageTime
			;



#ifdef VISUALS
		class_<visuals::Display, boost::noncopyable>("Display", init<int, int, double, Box*, PartSpecs*, SystemTrajectory*, bool>())
			.def("run", &visuals::Display::run)
			;
#endif

	}




}
