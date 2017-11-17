#include "MonoZimm.h"


MonoZimm::MonoZimm() : PartSpecs(1)
{
	name = "MonoZimm";
}


MonoZimm::MonoZimm(double sig, double A, double C, double friction, double mass) : MonoZimm()
{
	m_A = A;
	m_C = C;

	this->partTypes.getPartTypes().at(0).name = "F";
	this->partTypes.getPartTypes().at(0).mass = mass;
	this->partTypes.getPartTypes().at(0).sig = sig;
	this->partTypes.getPartTypes().at(0).friction = friction;


	build();
}


void MonoZimm::build()
{
	const double& sig = this->partTypes.getPartTypes().at(0).sig;
	const double& friction = this->partTypes.getPartTypes().at(0).friction;
	// inter particle forces
	ZimmermannRepAdh* repAd_AA(new ZimmermannRepAdh{ m_A, sig, m_C });
	addInterForce(0, repAd_AA);
}



bool MonoZimm::load(Hdf5* file, const char* groupName)
{
	try {
		H5::Group group = file->openGroup(groupName);

		std::string readName = file->readAttributeString(groupName, "name");
		// check if name is the same
		if (readName != name)
		{
			std::cerr << "Error: Trying to load a '" << readName << "' SystemSpecs into a '" << name << "' SystemSpecs." << std::endl;
			exit(EXIT_FAILURE);
		}

		m_A = file->readAttributeDouble(groupName, "A");
		m_C = file->readAttributeDouble(groupName, "C");

		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types");
		this->partTypes.load(file, partTypesGroupName);

		build();
		return true;
	}
	catch (H5::Exception &err)
	{
		err.printError(); // TODO: logger
		return false;
	}
	catch (std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		return false;
	}
}



bool MonoZimm::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "A", m_A);
		file->writeAttributeDouble(groupName, "C", m_C);

		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types");
		this->partTypes.save(file, partTypesGroupName);

		return true;
	}
	catch (H5::Exception &err)
	{
		err.printError(); // TODO: logger
		return false;
	}
}


bool MonoZimm::cellIsDead(const Cell* cell, const Box* box)
{
	return false;
}


bool MonoZimm::cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
	return false;
}


bool MonoZimm::cellIsBroken(const Cell* cell, const Box* box) const
{
	// cells are never broken in this model
	return false;
}