#include "Hdf5types.h"

// order is important (partType is constructed from the vectorType)

//H5::CompType Hdf5types::m_vectorType = Hdf5types::getVectorType();
H5::CompType* Hdf5types::m_partType = nullptr;
H5::CompType* Hdf5types::m_partTypewCell = nullptr;
H5::CompType* Hdf5types::m_simonType = nullptr;


Hdf5types::Hdf5types() {}

Hdf5types::~Hdf5types()
{
	delete m_partType;
	delete m_partTypewCell;
	delete m_simonType;
}


const H5::CompType& Hdf5types::getPartType()
{
	struct Initializer {
		Initializer() {
			m_partType = new H5::CompType(sizeof(LightPart));
			m_partType->insertMember("x",  HOFFSET(LightPart,  x), H5::PredType::NATIVE_DOUBLE);
			m_partType->insertMember("y",  HOFFSET(LightPart,  y), H5::PredType::NATIVE_DOUBLE);
			m_partType->insertMember("vx", HOFFSET(LightPart, vx), H5::PredType::NATIVE_DOUBLE);
			m_partType->insertMember("vy", HOFFSET(LightPart, vy), H5::PredType::NATIVE_DOUBLE);
		}
	};
	static Initializer ListInitializationGuard;
	return *m_partType;
}


const H5::CompType& Hdf5types::getPartTypewCell()
{
	struct Initializer {
		Initializer() {
			m_partTypewCell = new H5::CompType(sizeof(LightPartwCell));
			m_partTypewCell->insertMember("x" , HOFFSET(LightPartwCell, x),    H5::PredType::NATIVE_DOUBLE);
			m_partTypewCell->insertMember("y" , HOFFSET(LightPartwCell, y),    H5::PredType::NATIVE_DOUBLE);
			m_partTypewCell->insertMember("vx", HOFFSET(LightPartwCell, vx),   H5::PredType::NATIVE_DOUBLE);
			m_partTypewCell->insertMember("vy", HOFFSET(LightPartwCell, vy),   H5::PredType::NATIVE_DOUBLE);
			m_partTypewCell->insertMember( "c", HOFFSET(LightPartwCell, cell), H5::PredType::NATIVE_HSIZE);
		}
	};
	static Initializer ListInitializationGuard;
	return *m_partTypewCell;
}

const H5::CompType& Hdf5types::getSimonType()
{
	struct Initializer {
		Initializer() {
			m_simonType = new H5::CompType(sizeof(Vector));
			m_simonType->insertMember("x", HOFFSET(Vector, x), H5::PredType::NATIVE_DOUBLE);
			m_simonType->insertMember("y", HOFFSET(Vector, y), H5::PredType::NATIVE_DOUBLE);
		}
	};
	static Initializer ListInitializationGuard;
	return *m_simonType;
}