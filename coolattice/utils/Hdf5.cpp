#include "Hdf5.h"

bool Hdf5::exists(const char* name) const
{
	try {
		H5::Group group = openGroup(name);
	}
	catch (H5::FileIException &err)
	{
		return false;
	}
	return true;
}

bool Hdf5::loadObject(BinarySavable* object, const char* name)
{
	return object->load(this, name);
}

bool Hdf5::saveObject(BinarySavable* object, const char* name)
{
	return object->save(this, name);
}

void Hdf5::writeAttributeBool(const char* name, const char* attribute, bool value)
{
	H5::Group grp = openGroup(name);
	H5::Attribute a = grp.createAttribute(attribute, H5::PredType::NATIVE_HBOOL, H5::DataSpace());
	a.write(H5::PredType::NATIVE_HBOOL, &value);
}

bool Hdf5::readAttributeBool(const char* name, const char* attribute)
{
	H5::Group grp = openGroup(name);
	H5::Attribute a = grp.openAttribute(attribute);
	bool value;
	a.read(H5::PredType::NATIVE_HBOOL, &value);
	return value;
}


void Hdf5::writeAttributeInteger(const char* name, const char* attribute, int value)
{
	H5::Group grp = openGroup(name);
	H5::Attribute a = grp.createAttribute(attribute, H5::PredType::NATIVE_INT, H5::DataSpace());
	a.write(H5::PredType::NATIVE_INT, &value);
}

int Hdf5::readAttributeInteger(const char* name, const char* attribute)
{
	H5::Group grp = openGroup(name);
	H5::Attribute a = grp.openAttribute(attribute);
	int value;
	a.read(H5::PredType::NATIVE_INT, &value);
	return value;
}


void Hdf5::writeAttributeDouble(const char* name, const char* attribute, double value)
{
	H5::Group grp = openGroup(name);
	H5::Attribute a = grp.createAttribute(attribute, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());
	a.write(H5::PredType::NATIVE_DOUBLE, &value);
}

double Hdf5::readAttributeDouble(const char* name, const char* attribute)
{
	H5::Group grp = openGroup(name);
	H5::Attribute a = grp.openAttribute(attribute);
	double value;
	a.read(H5::PredType::NATIVE_DOUBLE, &value);
	return value;
}


void   Hdf5::writeAttributeString(const char* name, const char* attribute, const std::string& value)
{
	H5::Group grp = openGroup(name);
	
	H5::StrType strdatatype(H5::PredType::C_S1, 64);
	H5::DataSpace attr_dataspace = H5::DataSpace(H5S_SCALAR);
	const H5std_string strwritebuf(value.c_str());
	H5::Attribute a = grp.createAttribute(attribute, strdatatype, attr_dataspace);
	a.write(strdatatype, strwritebuf);

}

std::string Hdf5::readAttributeString(const char* name, const char* attribute)
{
	H5::Group grp = openGroup(name);
	H5::Attribute a = grp.openAttribute(attribute);

	H5::StrType strdatatype(H5::PredType::C_S1, 64);
	H5::DataSpace attr_dataspace = H5::DataSpace(H5S_SCALAR);
	
	H5std_string strreadbuf("");
	a.read(strdatatype, strreadbuf);

	return strreadbuf;
}


const char* Hdf5::readName(const char* name)
{
	return readAttributeString(name, "name").c_str();
}

H5::Group Hdf5::createGroupNoThrow(const char* name)
{
	try
	{
		H5::Group group = H5File::createGroup(name);
		return group;
	}
	catch (H5::FileIException &err) {
		// TODO: print to a logger
		this->unlink(name);
		H5::Group group = H5File::createGroup(name);
		return group;
	}
}