#include "ActiveParticles.h"


ActiveParticles::ActiveParticles()
{

}


void ActiveParticles::build()
{
	const double& sig     = this->partTypes.at(0).getPartTypes().at(0).sig;
	const double& friction = this->partTypes.at(0).getPartTypes().at(0).friction;
	const double& mass = this->partTypes.at(0).getPartTypes().at(0).mass;


	LJForce* lja(new LJForce{ sig, m_epsilon, (sig * sig * m_cut * m_cut) });
	ConstantPropulsionForce* activeForce(new ConstantPropulsionForce{ m_motility });

	addInterForce(0, 0, lja);
	addIntraForce(0, 0, activeForce);

	// noise, give by the temperature
	GaussianRandomForce* noise(new GaussianRandomForce{1.0, m_deltat, friction, mass, m_temperature});
	addOneBodyForce(0, 0, noise);
	
	cycleManager = new SingleStage{};
}


ActiveParticles::ActiveParticles(double epsilon, double motility, double cut, double friction, double mass, double temperature, double deltat)
{
	m_epsilon = epsilon;
	m_motility = motility;
	m_cut = cut;
	m_temperature = temperature;
	m_deltat = deltat;
	m_numberOfStages = 1;

	this->init(1, m_numberOfStages, specsName);

	std::cout << "build name: " << this->name << std::endl;

	this->partTypes.at(0).getPartTypes().at(0).name = "F";
	this->partTypes.at(0).getPartTypes().at(0).mass = mass;
	this->partTypes.at(0).getPartTypes().at(0).sig = 1.0;
	this->partTypes.at(0).getPartTypes().at(0).friction = friction;


	build();
}


bool ActiveParticles::load(Hdf5* file, const char* groupName)
{
	try {
		H5::Group group = file->openGroup(groupName);

		std::string readName = file->readAttributeString(groupName, "name");
		// check if name is the same
		if (readName != specsName)
		{
			std::cerr << "Error: Trying to load a '" << readName << "' SystemSpecs into a '" << specsName << "' SystemSpecs." << std::endl;
			exit(EXIT_FAILURE);
		}

		m_epsilon = file->readAttributeDouble(groupName, "epsilon");
		m_motility = file->readAttributeDouble(groupName, "motility");
		m_cut = file->readAttributeDouble(groupName, "cut");
		m_temperature = file->readAttributeDouble(groupName, "temperature");
		m_deltat = file->readAttributeDouble(groupName, "deltat");


		this->init(1, 1, specsName);

		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(0).load(file, partTypesGroupName);

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



bool ActiveParticles::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "motility", m_motility);
		file->writeAttributeDouble(groupName, "epsilon", m_epsilon);
		file->writeAttributeDouble(groupName, "cut", m_cut);
		file->writeAttributeDouble(groupName, "temperature", m_temperature);
		file->writeAttributeDouble(groupName, "deltat", m_deltat);
		
		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(0).save(file, partTypesGroupName);

		return true;
	}
	catch (H5::Exception &err)
	{
		err.printError(); // TODO: logger
		return false;
	}
}


bool ActiveParticles::cellIsBroken(const Cell* cell, const Box* box) const
{
	// active particles are never broken
	return false;
}

bool ActiveParticles::cellIsDead(const Cell* cell, const Box* box)
{
	// active particles never die
	return false;
}

bool ActiveParticles::divisionCriterion(Cell* cell, const Box* box) const
{
	// active particles never divide
	return false;
}


bool ActiveParticles::cellDivides(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
	// active particles never divide
	return false;
}


bool ActiveParticles::endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const
{
	// active particles never succeed to divide because they never try to
	return false;
}


