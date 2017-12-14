#pragma once

#include "Hdf5.h"
#include "../math/Vector.h"
#include "../particles/Part.h"

// contains compound types defined by the user
class Hdf5types
{
private:
	static H5::CompType* m_partType;
	static H5::CompType* m_partTypewCell;
	static H5::CompType* m_simonType;

public:
	static const H5::CompType& getPartType();
	static const H5::CompType& getPartTypewCell();
	static const H5::CompType& getSimonType();

	Hdf5types();
	~Hdf5types();

	// Ownership is not to be shared ...
	Hdf5types(const Hdf5types&) = delete;
	Hdf5types& operator=(const Hdf5types&) = delete;
	// ... or transferred (this is just for lazyness)
	Hdf5types(Hdf5types&& other) = delete;
	Hdf5types& operator=(Hdf5types&& other) = delete;
};

