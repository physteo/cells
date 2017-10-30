#include "SMTYSpecsSticky.h"

SMTYSpecsSticky::SMTYSpecsSticky() : PartSpecs(2)
{
	name = "SMTY_Sticky";
}

void SMTYSpecsSticky::build()
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


	Sigmoidal* sigmoidalAArepulsion(new Sigmoidal{m_a1, m_b1, m_gamma1, sigAA, (sigAA *   sigAA * m_cut * m_cut) });
	Sigmoidal* sigmoidalABrepulsion(new Sigmoidal{m_a1, m_b1, m_gamma1, sigAB, (sigAB *   sigAB * m_cut * m_cut) });
	Sigmoidal* sigmoidalBArepulsion(new Sigmoidal{m_a1, m_b1, m_gamma1, sigAB, (sigAB *   sigAB * m_cut * m_cut) });
	Sigmoidal* sigmoidalBBrepulsion(new Sigmoidal{m_a1, m_b1, m_gamma1, sigBB, (sigBB *   sigBB * m_cut * m_cut) });
	
	addInterForce(0, sigmoidalAArepulsion);
	addInterForce(1, sigmoidalABrepulsion);
	addInterForce(2, sigmoidalABrepulsion);
	addInterForce(3, sigmoidalBBrepulsion);
	
	Sigmoidal* sigmoidalAAattraction(new Sigmoidal{ m_a2, m_b2, m_gamma2, sigAA*m_sigma2factor, (sigAA *   sigAA * m_cut * m_cut) });
	Sigmoidal* sigmoidalABattraction(new Sigmoidal{ m_a2, m_b2, m_gamma2, sigAB*m_sigma2factor, (sigAB *   sigAB * m_cut * m_cut) });
	Sigmoidal* sigmoidalBAattraction(new Sigmoidal{ m_a2, m_b2, m_gamma2, sigAB*m_sigma2factor, (sigAB *   sigAB * m_cut * m_cut) });
	Sigmoidal* sigmoidalBBattraction(new Sigmoidal{ m_a2, m_b2, m_gamma2, sigBB*m_sigma2factor, (sigBB *   sigBB * m_cut * m_cut) });
	
	addInterForce(0, sigmoidalAAattraction);
	addInterForce(1, sigmoidalABattraction);
	addInterForce(2, sigmoidalABattraction);
	addInterForce(3, sigmoidalBBattraction);


	FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
	FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });

	CilForce*  cil(new CilForce{ m_motility });

	addIntraForce(0, feneAA);
	addIntraForce(1, feneAB);
	addIntraForce(1, cil);
	addIntraForce(2, feneBA);
	addIntraForce(3, feneBB);

	GaussianRandomForce* random(new GaussianRandomForce{ 1.0, 0.001,1.0,1.0,5.0e-1 });

	this->oneBodyForces.at(0).push_back(random); // TODO URGENT: pass dt etc
	this->oneBodyForces.at(1).push_back(random); // TODO URGENT: pass dt etc

	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << ssCellLength << std::endl;
	std::cout << " - Spees at ss = " << ssSpeed << std::endl;
	std::cout << " - Migration t = " << migrationTime << std::endl;
}




SMTYSpecsSticky::SMTYSpecsSticky(const Parameters* params) : SMTYSpecsSticky()
{

	if (params->size() != 17)
	{
		std::cerr << "SMTYSpecsSticky: not enough parameters." << std::endl;
		assert(false);
	}


	double sigAA        = params->getParam(0);
	double sigBB        = params->getParam(1);
	double a1           = params->getParam(2);
	double a2           = params->getParam(3);
	double b1           = params->getParam(4);
	double b2           = params->getParam(5);
	double gamma1       = params->getParam(6);
	double gamma2       = params->getParam(7);
	double sigma2factor = params->getParam(8);
	double motility     = params->getParam(9);
	double rMaxSquared  = params->getParam(10);
	double kappa	    = params->getParam(11);
	double frictionF    = params->getParam(12);
	double frictionB    = params->getParam(13);
	double massF        = params->getParam(14);
	double massB        = params->getParam(15);
	double cut          = params->getParam(16);


	m_motility = motility;
	m_cut = cut;
	m_rMaxSquared = rMaxSquared;
	m_kappa = kappa;

	m_a1 = a1;
	m_a2 = a2;
	m_b1 = b1;
	m_b2 = b2;
	m_gamma1 = gamma1;
	m_gamma2 = gamma2;
	m_sigma2factor = sigma2factor;


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



bool SMTYSpecsSticky::load(Hdf5* file, const char* groupName)
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

		m_motility = file->readAttributeDouble(groupName, "motility");
		//m_epsilon = file->readAttributeDouble(groupName, "eps");
		m_cut = file->readAttributeDouble(groupName, "cut");
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



bool SMTYSpecsSticky::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "motility", m_motility);
		//file->writeAttributeDouble(groupName, "eps", m_epsilon);
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



bool SMTYSpecsSticky::cellIsBroken(const Cell* cell, const Box* box) const
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


bool SMTYSpecsSticky::cellIsDead(const Cell* cell, const Box* box) const
{
	// in this system, a cell cannot die


	// a cell is dead when it's length is
	// bigger than Rmax2 / 8.0
	//Vector vectorDistance;
	//double distance2 = box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, vectorDistance);
	//if (distance2 > m_rMaxSquared / 8.0)
	//{
	//	std::cout << "cell is dead" << std::endl;
	//	return true;
	//}
	return false;
}


bool SMTYSpecsSticky::cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box) const
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