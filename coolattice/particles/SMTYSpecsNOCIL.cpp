#include "SMTYSpecsNOCIL.h"


SMTYSpecsNOCIL::SMTYSpecsNOCIL() : PartSpecs(2)
{
	name = "SMTY_NOCIL";
}

void SMTYSpecsNOCIL::build()
{
	const double& sigAA = this->partTypes.getPartTypes().at(0).sig;
	const double& sigBB = this->partTypes.getPartTypes().at(1).sig;
	const double& frictionF = this->partTypes.getPartTypes().at(0).friction;
	const double& frictionB = this->partTypes.getPartTypes().at(1).friction;

	double sigAB = .5*(sigAA + sigBB);

	// characteristics
	double ssCellLength = sqrt(m_rMaxSquared) * sqrt(1.0 - m_kappa * (1.0 / frictionF + 1.0 / frictionB) / (m_motility / frictionF));
	double ssSpeed = ssCellLength * m_motility / (frictionF + frictionB);
	double migrationTime = sqrt(m_rMaxSquared) / ssSpeed;



	LJForce* lja(new LJForce{ sigAA, m_epsilon, (sigAA * sigAA * m_cut * m_cut) });
	LJForce* ljb(new LJForce{ sigAB, m_epsilon, (sigAB * sigAB * m_cut * m_cut) });
	LJForce* ljc(new LJForce{ sigAB, m_epsilon, (sigAB * sigAB * m_cut * m_cut) });
	LJForce* ljd(new LJForce{ sigBB, m_epsilon, (sigBB * sigBB * m_cut * m_cut) });

	std::vector< std::vector< TwoBodyForce* > >& interForces = this->twoBodyForces.getInterForces();
	interForces.at(0).push_back(lja);
	interForces.at(1).push_back(ljb);
	interForces.at(2).push_back(ljc);
	interForces.at(3).push_back(ljd);


	//FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
	TwoBodyNoForce*   noforce(new TwoBodyNoForce{});
	//FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
	ConstantPropulsionForce*  propForce(new ConstantPropulsionForce{ m_motility * ssCellLength });

	std::vector< std::vector<TwoBodyForce* > >& intraForces = this->twoBodyForces.getIntraForces();
	intraForces.at(0).push_back(noforce); // this is not necessary
	intraForces.at(3).push_back(noforce); // this is not necessary

	// interaction where part1 is the front, and part2 is the back: both Fene force and CIL force to part1
	intraForces.at(1).push_back(feneAB);
	intraForces.at(1).push_back(propForce);
	//intraForces.at(1).push_back(std::make_unique<ConstantPropulsionForce>(std::move(ConstantPropulsionForce{ motility * ssSpeed})));

	// interaction where part2 is the front and part1 is the back: no CIL given to part1 
	intraForces.at(2).push_back(feneBA);

	//partSpecs.oneBodyForces.at(0).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7})))); // TODO URGENT: pass dt etc
	//partSpecs.oneBodyForces.at(1).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7 })))); // TODO URGENT: pass dt etc





	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << ssCellLength << std::endl;
	std::cout << " - Spees at ss = " << ssSpeed << std::endl;
	std::cout << " - Migration t = " << migrationTime << std::endl;
}

SMTYSpecsNOCIL::SMTYSpecsNOCIL(double sigAA, double sigBB, double motility,
	double epsilon, double cut, double rMaxSquared, double kappa, double frictionF, double frictionB, double massF, double massB) : SMTYSpecsNOCIL()
{
	m_motility = motility;
	m_epsilon = epsilon;
	m_cut = cut;
	m_rMaxSquared = rMaxSquared;
	m_kappa = kappa;

	// particle types
	this->partTypes.getPartTypes().at(0).name = "F";
	this->partTypes.getPartTypes().at(1).name = "B";
	this->partTypes.getPartTypes().at(0).mass = massF;
	this->partTypes.getPartTypes().at(1).mass = massB;
	this->partTypes.getPartTypes().at(0).sig = sigAA;
	this->partTypes.getPartTypes().at(1).sig = sigBB;
	this->partTypes.getPartTypes().at(0).friction = frictionF;
	this->partTypes.getPartTypes().at(1).friction = frictionB;
	build();
}


bool SMTYSpecsNOCIL::load(Hdf5* file, const char* groupName)
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

		m_motility		= file->readAttributeDouble(groupName, "motility");
		m_epsilon		= file->readAttributeDouble(groupName, "eps");
		m_cut			= file->readAttributeDouble(groupName, "cut");
		m_rMaxSquared	= file->readAttributeDouble(groupName, "rMax2");
		m_kappa			= file->readAttributeDouble(groupName, "k");

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



bool SMTYSpecsNOCIL::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "motility", m_motility);
		file->writeAttributeDouble(groupName, "eps", m_epsilon);
		file->writeAttributeDouble(groupName, "cut", m_cut);
		file->writeAttributeDouble(groupName, "rMax2", m_rMaxSquared);
		file->writeAttributeDouble(groupName, "k", m_kappa);

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


bool SMTYSpecsNOCIL::cellIsBroken(const Cell* cell, const Box* box) const
{
	// in the SMTY system a cell is broken when it's length is
	// bigger than Rmax
	Vector vectorDistance;
	double distance2 = box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, vectorDistance);
	if (distance2 > m_rMaxSquared)
	{
		std::cerr << "Broken cell detected." << std::endl;
		return true;
	}
	else if (std::isnan(distance2))
	{
		std::cerr << "Cell distance is NaN." << std::endl;
		return true;
	}

	return false;

}


bool SMTYSpecsNOCIL::cellIsDead(const Cell* cell, const Box* box)
{
	return false;
}


bool SMTYSpecsNOCIL::cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
	return false;
}