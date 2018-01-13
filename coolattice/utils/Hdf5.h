#pragma once

#include "H5Cpp.h"
#include <iostream>
class Hdf5;

class BinarySavable
{
public:
	virtual bool load(Hdf5* file, const char* name)       = 0;
	virtual bool save(Hdf5* file, const char* name) const = 0;
};


class Hdf5 : private H5::H5File
{
public:
	Hdf5(const char* fileName, unsigned int flags) : H5::H5File(fileName, flags) {}
	void closeFile();
	void createNewGroup(const char* name) { createGroupNoThrow(name); }
	H5::Group openGroup(const char* name) const { return H5::H5File::openGroup(name); }
	hsize_t getNumObjs() const { return H5::H5File::getNumObjs(); }
	
	inline void unlinkFile(const char* name) { H5File::unlink(name); }
	bool exists(const char* name) const;

	bool   loadObject(BinarySavable* object, const char* name);
	bool   saveObject(BinarySavable* object, const char* name);
	
	void   writeAttributeBool(const char* name, const char* attribute, bool value);
	bool   readAttributeBool(const char* name, const char* attribute);
	void   writeAttributeInteger(const char* name, const char* attribute, int value);
	int    readAttributeInteger(const char* name, const char* attribute);
	void   writeAttributeDouble(const char* name, const char* attribute, double value);
	double readAttributeDouble(const char* name, const char* attribute);
	void   writeAttributeString(const char* name, const char* attribute, const std::string& value);
	std::string readAttributeString(const char* name, const char* attribute);

	const char* readName(const char* name);

public:
	H5::Group  createGroupNoThrow(const char*);

};


//class Hdf5types
//{
//
//};
