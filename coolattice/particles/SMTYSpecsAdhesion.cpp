#include "SMTYSpecsAdhesion.h"

SMTYSpecsAdhesion::SMTYSpecsAdhesion() : PartSpecs(2)
{
	name = "SMTY_ZimmermannRepAdh";
}

SMTYSpecsAdhesion::SMTYSpecsAdhesion(double sigAA, double sigBB, double A, double C, double motility,
	double rMaxSquared, double kappa, double frictionF, double frictionB, double massF, double massB) : SMTYSpecsAdhesion()
{
	m_A = A;
	m_C = C;
	m_motility = motility;
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

void SMTYSpecsAdhesion::build()
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


    // inter particle forces
	ZimmermannRepAdh* repAd_AA(new ZimmermannRepAdh{ m_A, sigAA, m_C });
	ZimmermannRepAdh* repAd_AB(new ZimmermannRepAdh{ m_A, sigAB, m_C });
	ZimmermannRepAdh* repAd_BB(new ZimmermannRepAdh{ m_A, sigBB, m_C });
	
	addInterForce(0, repAd_AA);
	addInterForce(1, repAd_AB);
	addInterForce(2, repAd_AB);
	addInterForce(3, repAd_BB);

	// intra particle forces
	FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });
	CilForce*  cil(new CilForce{ m_motility });

	addIntraForce(0, feneAA); // this is not necessary
	addIntraForce(3, feneAA); // this is not necessary
	addIntraForce(1, feneAB); // interaction where part1 is the front, and part2 is the back: both Fene force and CIL force to part1
	addIntraForce(1, cil);	  // fene is in the slot 0 ! (keep in mind when measuring forces
							  // CIL  is in the slot 1 ! ( //	 //    //   //    //  )
							  
	addIntraForce(2, feneBA); // interaction where part2 is the front and part1 is the back: no CIL given to part1 


	// one body particle forces
	//PlanarWall* wall1(new PlanarWall{ 1.0,1.0e-4,2.0,0,13.5 });
	//PlanarWall* wall2(new PlanarWall{ 1.0,1.0e-4,2.0,0,38 });
	//addOneBodyForce(0, wall1);
	//addOneBodyForce(0, wall2);
	//addOneBodyForce(1, wall1);
	//addOneBodyForce(1, wall2);


	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << ssCellLength << std::endl;
	std::cout << " - Spees at ss = " << ssSpeed << std::endl;
	std::cout << " - Migration t = " << migrationTime << std::endl;
}



bool SMTYSpecsAdhesion::load(Hdf5* file, const char* groupName)
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

		m_motility = file->readAttributeDouble(groupName, "motility");
		m_rMaxSquared = file->readAttributeDouble(groupName, "rMax2");
		m_kappa = file->readAttributeDouble(groupName, "k");

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



bool SMTYSpecsAdhesion::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "A", m_A);
		file->writeAttributeDouble(groupName, "C", m_C);
		file->writeAttributeDouble(groupName, "motility", m_motility);
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



bool SMTYSpecsAdhesion::cellIsBroken(const Cell* cell, const Box* box) const
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


bool SMTYSpecsAdhesion::cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box) const
{
	// in this model, a cell can duplicate if its length is bigger than Rmax/2.0
	Vector vectorDistance;
	double distance2 = box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, vectorDistance);

	if (distance2 > m_rMaxSquared / 8.0)
	{
		// bla
		Cell newcell{ 2 };
		newcell.getPart(0).type = 0;
		newcell.getPart(0).cell = 373;

		newcell.getPart(0).position = cell->getPart(1).position;

		newcell.getPart(1).type = 1;
		newcell.getPart(1).cell = 373;
		newcell.getPart(1).position = cell->getPart(1).position;

		cell->getPart(1).position = cell->getPart(0).position;

		// set velocities to zero
		newcell.getPart(0).velocity = Vector{ 0.0, 0.0 };
		newcell.getPart(1).velocity = Vector{ 0.0, 0.0 };
		cell->getPart(0).velocity = Vector{ 0.0,0.0 };
		cell->getPart(1).velocity = Vector{ 0.0,0.0 };

		newCells->push_back(newcell);
		return true;
	}
	return false;
}