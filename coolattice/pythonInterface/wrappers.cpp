#include "wrappers.h"


namespace pywrapper {
	using namespace boost::python;

	BOOST_PYTHON_MODULE(Cellulae) {
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

		class_<CellColony>("CellColony", init<>())
			.def("save", &CellColony::save)
			.def("load", &CellColony::load)
			.def("populate", &CellColony::populate)
			.def("addTwoPartsCell", &CellColony::addTwoPartsCell )
			.def("size", &CellColony::size)
			.def("totalNumberOfParts", &CellColony::totalNumberOfParts)
			.def("getPartX", &CellColony::getPartX)
			.def("getPartY", &CellColony::getPartY)
			;

		class_<Box>("Box", init<>())
			.def(init<double, double, bool, int, int>())
			.def("clearSubBoxes", &Box::clearSubBoxes)
			.def("save", &Box::save)
			.def("load", &Box::load)
			;

		//class_<PartSpecs>("PartSpecs", no_init)
		//	;

		class_<PartSpecsWrap, boost::noncopyable>("PartSpecs")
			.def("setFriction",  &PartSpecs::setFriction)
			.def("load", pure_virtual(&PartSpecs::load))
			.def("save", pure_virtual(&PartSpecs::save))
			.def("cellIsBroken", pure_virtual(&PartSpecs::cellIsBroken))
			;

		class_<SMTYSpecs, bases<PartSpecs> >("SMTYSpecs", init<>())
			.def(init<double, double, double, double, double, double, double, double, double, double, double>())
			.def("save", &SMTYSpecs::save)
			.def("load", &SMTYSpecs::load)
			.def("cellIsBroken", &SMTYSpecs::cellIsBroken)
			;

		class_<SMTYSpecsNOCIL, bases<PartSpecs> >("SMTYSpecsNOCIL", init<>())
			.def(init<double, double, double, double, double, double, double, double, double, double, double>())
			.def("save", &SMTYSpecsNOCIL::save)
			.def("load", &SMTYSpecsNOCIL::load)
			.def("cellIsBroken", &SMTYSpecs::cellIsBroken)
			;

		class_<MeasureTwoBodyForce>("MeasureTwoBodyForce", init<PartSpecs*, size_t, size_t, bool, size_t>())
			.def("save", &MeasureTwoBodyForce::save)
			.def("load", &MeasureTwoBodyForce::load)
			;

		class_<System>("System", init<CellColony*, PartSpecs*, Box*>())
			.def("updatePositions", &System::updatePositions)
			.def("computeForces", &System::computeForces)
			.def("collectForces",  &System::collect)
			.def("getColony", &System::getColony)
			.def("cellsAreBroken", &System::cellsAreBroken)
			.def("setTypeFriction", &System::setTypeFriction)
			.def("getTypeFriction", &System::getTypeFriction)
			.def("registerTwoBodyForceMeasurement", &System::registerTwoBodyForceMeasurement)
			;


		class_<SystemTrajectory>( "SystemTrajectory", init<>())
			.def("save", &SystemTrajectory::save)
			.def("load", &SystemTrajectory::load)
			.def("size", &SystemTrajectory::size)
			.def("push_back", &SystemTrajectory::push_back)
			.def("copyColony", &SystemTrajectory::copyColony)
			;



#ifdef VISUALS
		class_<visuals::Display, boost::noncopyable>("Display", init<int, int, Box*, PartSpecs*, SystemTrajectory*, bool>())
			.def("run", &visuals::Display::run)
			;
#endif

	}




}
