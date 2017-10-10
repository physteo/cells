#include "SMTYSpecsNOCIL.h"


SMTYSpecsNOCIL::SMTYSpecsNOCIL() : PartSpecs(2)
{

}

void SMTYSpecsNOCIL::build()
{
	double sigAB = .5*(m_sigAA + m_sigBB);

	// characteristics
	double ssCellLength = sqrt(m_rMaxSquared) * sqrt(1.0 - 2.0 * m_kappa / m_motility);
	double ssSpeed = ssCellLength * m_motility / (2 * m_friction);
	double migrationTime = sqrt(m_rMaxSquared) / ssSpeed;


	LJForce* lja(new LJForce{ m_sigAA, m_epsilon, (m_sigAA * m_sigAA * m_cut * m_cut) });
	LJForce* ljb(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
	LJForce* ljc(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
	LJForce* ljd(new LJForce{ m_sigBB, m_epsilon, (m_sigBB * m_sigBB * m_cut * m_cut) });

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

	this->partTypes.at(0).name = "F";
	this->partTypes.at(1).name = "B";
	this->partTypes.at(0).mass = 1.0;
	this->partTypes.at(1).mass = 1.0;
	this->partTypes.at(0).sig = m_sigAA;
	this->partTypes.at(1).sig = m_sigBB;

	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << ssCellLength << std::endl;
	std::cout << " - Spees at ss = " << ssSpeed << std::endl;
	std::cout << " - Migration t = " << migrationTime << std::endl;
}

SMTYSpecsNOCIL::SMTYSpecsNOCIL(double sigAA, double sigBB, double motility,
	double epsilon, double cut, double rMaxSquared, double kappa, double friction) : PartSpecs(2),
	m_sigAA(sigAA), m_sigBB(sigBB), m_motility(motility), m_epsilon(epsilon),
	m_cut(cut), m_rMaxSquared(rMaxSquared), m_kappa(kappa), m_friction(friction)
{
	build();
}


bool SMTYSpecsNOCIL::load(Hdf5* file, const char* groupName)
{
	try {
		H5::Group group = file->openGroup(groupName);
		m_sigAA = file->readAttributeDouble(groupName, "sigAA");
		m_sigBB = file->readAttributeDouble(groupName, "sigBB");
		m_motility = file->readAttributeDouble(groupName, "motility");
		m_epsilon = file->readAttributeDouble(groupName, "eps");
		m_cut = file->readAttributeDouble(groupName, "cut");
		m_rMaxSquared = file->readAttributeDouble(groupName, "rMax2");
		m_kappa = file->readAttributeDouble(groupName, "k");
		m_friction = file->readAttributeDouble(groupName, "friction");

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
		file->writeAttributeDouble(groupName, "sigAA", m_sigAA);
		file->writeAttributeDouble(groupName, "sigBB", m_sigBB);
		file->writeAttributeDouble(groupName, "eps", m_epsilon);
		file->writeAttributeDouble(groupName, "motility", m_motility);
		file->writeAttributeDouble(groupName, "cut", m_cut);
		file->writeAttributeDouble(groupName, "rMax2", m_rMaxSquared);
		file->writeAttributeDouble(groupName, "k", m_kappa);
		file->writeAttributeDouble(groupName, "friction", m_friction);

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