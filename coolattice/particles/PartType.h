#pragma once
#include <iostream>
#include <string>
#include "../utils/Hdf5.h"

struct PartType
{
	std::string name;
	double mass;
	double sig;
	double friction;
};

class PartTypeVector : BinarySavable
{
	std::vector<PartType> partTypes;
public:
	PartTypeVector() : partTypes(0) {}
	const std::vector<PartType>& getPartTypes() const { return partTypes; }
	std::vector<PartType>& getPartTypes() { return partTypes; }

	virtual bool load(Hdf5* file, const char* groupName)
	{

		try {
			H5::Group group = file->openGroup(groupName);
			
			// how many part types are there?
			size_t numberOfPartTypes = file->readAttributeInteger(groupName, "numberOfPartTypes");
			this->partTypes.resize(numberOfPartTypes);

			for (size_t t = 0; t < partTypes.size(); t++)
			{
				char nameString[1024];
				char sigString[1024];
				char massString[1024];
				char frictionString[1024];

				sprintf(nameString,     "%s_%u", "name", (unsigned int)t);
				sprintf(sigString,      "%s_%u", "sig", (unsigned int)t);
				sprintf(massString,     "%s_%u", "mass", (unsigned int)t);
				sprintf(frictionString, "%s_%u", "friction", (unsigned int)t);

				//partTypes.at(t).name = "cacca";
				partTypes.at(t).name = file->readAttributeString(groupName, nameString);
				partTypes.at(t).sig = file->readAttributeDouble(groupName, sigString);
				partTypes.at(t).mass = file->readAttributeDouble(groupName, massString);
				partTypes.at(t).friction = file->readAttributeDouble(groupName, frictionString);


			}

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



	virtual bool save(Hdf5* file, const char* groupName) const
	{
		try {
			file->createNewGroup(groupName);
			H5::Group group = file->openGroup(groupName);
			file->writeAttributeInteger(groupName, "numberOfPartTypes", this->getPartTypes().size());

			for (size_t t = 0; t < partTypes.size(); t++)
			{
				const PartType& pt = this->partTypes.at(t);
				
				char nameString[1024];
				char sigString[1024];
				char massString[1024];
				char frictionString[1024];

				sprintf(nameString,     "%s_%u", "name",     (unsigned int)t);
				sprintf(sigString,      "%s_%u", "sig",      (unsigned int)t);
				sprintf(massString,     "%s_%u", "mass",     (unsigned int)t);
				sprintf(frictionString, "%s_%u", "friction", (unsigned int)t);

				file->writeAttributeString(groupName, nameString, pt.name);
				file->writeAttributeDouble(groupName, sigString, pt.sig );
				file->writeAttributeDouble(groupName, massString, pt.mass);
				file->writeAttributeDouble(groupName, frictionString, pt.friction);
			}
			
			return true;
		}
		catch (H5::Exception &err)
		{
			err.printError(); // TODO: logger
			return false;
		}
	}

};