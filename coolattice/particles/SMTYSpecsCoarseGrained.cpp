#include "SMTYSpecsCoarseGrained.h"


SMTYSpecsCoarseGrained::SMTYSpecsCoarseGrained()
{

}


void SMTYSpecsCoarseGrained::build()
{
	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << m_ssCellLength << std::endl;
	std::cout << " - Speed at ss = " << m_ssSpeed << std::endl;
	std::cout << " - Migration t = " << m_migrationTime << std::endl;

	// stage MIGRATION
	{
		const double& sigAA = this->partTypes.at(MIGRATION).getPartTypes().at(0).sig;
		const double& sigBB = this->partTypes.at(MIGRATION).getPartTypes().at(1).sig;
		const double& frictionF = this->partTypes.at(MIGRATION).getPartTypes().at(0).friction;
		const double& frictionB = this->partTypes.at(MIGRATION).getPartTypes().at(1).friction;

		double sigAB = .5*(sigAA + sigBB);


		LJForce* lja(new LJForce{ sigAA, m_epsilon, (sigAA *   sigAA * m_cut * m_cut) });
		LJForce* ljb(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljc(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljd(new LJForce{ sigBB, m_epsilon, (sigBB *   sigBB * m_cut * m_cut) });

		addInterForce(MIGRATION, 0, lja);
		addInterForce(MIGRATION, 1, ljb);
		addInterForce(MIGRATION, 2, ljc);
		addInterForce(MIGRATION, 3, ljd);

		FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });

		addIntraForce(MIGRATION, 0, feneAA);
		addIntraForce(MIGRATION, 1, feneAB);
		addIntraForce(MIGRATION, 2, feneBA);
		addIntraForce(MIGRATION, 3, feneBB);


		if (m_CIL == false) {
			ConstantPropulsionForce* nocil(new ConstantPropulsionForce{ m_motility * m_ssCellLength });
			addIntraForce(MIGRATION, 1, nocil);
		}
		else if (m_CIL == true)
		{
			CilForce*  cil(new CilForce{ m_motility });
			addIntraForce(MIGRATION, 1, cil);
		}
		// noise
		//partSpecs.oneBodyForces.at(0).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7})))); // TODO URGENT: pass dt etc
		//partSpecs.oneBodyForces.at(1).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7 })))); // TODO URGENT: pass dt etc
	}


	if (m_division) {
		// stage DIVISION

		const double& sigAA     = this->partTypes.at(DIVISION).getPartTypes().at(0).sig;
		const double& sigBB     = this->partTypes.at(DIVISION).getPartTypes().at(1).sig;
		const double& frictionF = this->partTypes.at(DIVISION).getPartTypes().at(0).friction;
		const double& frictionB = this->partTypes.at(DIVISION).getPartTypes().at(1).friction;

		double sigAB = .5*(sigAA + sigBB);


		LJForce* lja(new LJForce{ sigAA, m_epsilon, (sigAA *   sigAA * m_cut * m_cut) });
		LJForce* ljb(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljc(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljd(new LJForce{ sigBB, m_epsilon, (sigBB *   sigBB * m_cut * m_cut) });

		addInterForce(DIVISION, 0, lja);
		addInterForce(DIVISION, 1, ljb);
		addInterForce(DIVISION, 2, ljc);
		addInterForce(DIVISION, 3, ljd);

		FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });

		addIntraForce(DIVISION, 0, feneAA);
		addIntraForce(DIVISION, 1, feneAB);
		addIntraForce(DIVISION, 2, feneBA);
		addIntraForce(DIVISION, 3, feneBB);

		// propulsion force to both parts
		ConstantPropulsionForce* nocil(new ConstantPropulsionForce{ 2.0 * m_motility * m_ssCellLength }); // TODO urgent: remove hard coded value 2.0
		addIntraForce(DIVISION, 1, nocil);
		addIntraForce(DIVISION, 2, nocil);

		// cycle manager
		cycleManager = new SingleStageWithDivision{ m_divisionCycleLength, m_rateDuplication, DIVISION };

	}
	else {
		cycleManager = new SingleStage{};
	}




}


SMTYSpecsCoarseGrained::SMTYSpecsCoarseGrained(double sigAA, double sigBB, double motility,
	double epsilon, double cut, double rMaxSquared, double kappa,
	double frictionF, double frictionB, double massF, double massB,
	double rateDuplication, double thresholdDuplication,
	size_t divisionCycleLength, bool CIL, bool division)
{

	m_division = division;
	m_numberOfStages = division ? 2 : 1;

	this->init(2, m_numberOfStages, specsName);

	std::cout << "build name: " << this->name << std::endl;

	m_motility = motility;
	m_epsilon = epsilon;
	m_cut = cut;

	m_rMaxSquared = rMaxSquared;
	m_kappa = kappa;
	m_rateDuplication = rateDuplication;
	m_thresholdDuplication = thresholdDuplication;
	m_divisionCycleLength = divisionCycleLength;

	m_CIL = CIL;


	// stage Extension
	{
		this->partTypes.at(MIGRATION).getPartTypes().at(0).name = "F";
		this->partTypes.at(MIGRATION).getPartTypes().at(1).name = "B";
		this->partTypes.at(MIGRATION).getPartTypes().at(0).mass = massF;
		this->partTypes.at(MIGRATION).getPartTypes().at(1).mass = massB;
		this->partTypes.at(MIGRATION).getPartTypes().at(0).sig = sigAA;
		this->partTypes.at(MIGRATION).getPartTypes().at(1).sig = sigBB;



		this->partTypes.at(MIGRATION).getPartTypes().at(0).friction = frictionF;
		this->partTypes.at(MIGRATION).getPartTypes().at(1).friction = frictionB;

	}

	if (m_division) {

		this->partTypes.at(DIVISION).getPartTypes().at(0).name = "F";
		this->partTypes.at(DIVISION).getPartTypes().at(1).name = "B";
		this->partTypes.at(DIVISION).getPartTypes().at(0).mass = massF;
		this->partTypes.at(DIVISION).getPartTypes().at(1).mass = massB;
		this->partTypes.at(DIVISION).getPartTypes().at(0).sig = sigAA;
		this->partTypes.at(DIVISION).getPartTypes().at(1).sig = sigBB;
		this->partTypes.at(DIVISION).getPartTypes().at(0).friction = frictionF;
		this->partTypes.at(DIVISION).getPartTypes().at(1).friction = frictionB;
	}


	// characteristics
	m_ssCellLength = sqrt(m_rMaxSquared) * sqrt(1.0 - m_kappa * (1.0 / frictionF + 1.0 / frictionB) / (m_motility / frictionF));
	m_ssSpeed = m_ssCellLength * m_motility / (frictionF + frictionB);
	m_migrationTime = sqrt(m_rMaxSquared) / m_ssSpeed;


	build();
}


SMTYSpecsCoarseGrained::SMTYSpecsCoarseGrained(const Parameters* params, size_t divisionCycleLength, bool CIL, bool division) :
	SMTYSpecsCoarseGrained(params->getParam(0),
		params->getParam(1),
		params->getParam(2),
		params->getParam(3),
		params->getParam(4),
		params->getParam(5),
		params->getParam(6),
		params->getParam(7),
		params->getParam(8),
		params->getParam(9),
		params->getParam(10),
		params->getParam(11),
		params->getParam(12),
		divisionCycleLength, CIL, division)
{
}


bool SMTYSpecsCoarseGrained::load(Hdf5* file, const char* groupName)
{
	try {
		H5::Group group = file->openGroup(groupName);

		std::string readName = file->readAttributeString(groupName, "name");
		// check if name is the same
		if (readName != specsName)
		{
			std::cerr << "Error: Trying to load a '" << readName << "' SystemSpecs into a '" << name << "' SystemSpecs." << std::endl;
			exit(EXIT_FAILURE);
		}

		m_motility = file->readAttributeDouble(groupName, "motility");
		m_epsilon = file->readAttributeDouble(groupName, "epsilon");
		m_cut = file->readAttributeDouble(groupName, "cut");
		m_rMaxSquared = file->readAttributeDouble(groupName, "rMax2");
		m_kappa = file->readAttributeDouble(groupName, "k");
		m_rateDuplication = file->readAttributeDouble(groupName, "rateDuplication");
		m_thresholdDuplication = file->readAttributeDouble(groupName, "thresholdDuplication");
		m_CIL = file->readAttributeBool(groupName, "CIL");
		m_division = file->readAttributeBool(groupName, "division");

		this->init(2, m_division ? 3 : 2, specsName);

		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(MIGRATION).load(file, partTypesGroupName);

		if (m_division) {
			strcpy(partTypesGroupName, groupName);
			strcat(partTypesGroupName, "/Types_2");
			this->partTypes.at(DIVISION).load(file, partTypesGroupName);
		}

		m_ssCellLength = file->readAttributeDouble(groupName, "ssCellLength");
		m_ssSpeed = file->readAttributeDouble(groupName, "ssSpeed");
		m_migrationTime = file->readAttributeDouble(groupName, "migrationTime");

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



bool SMTYSpecsCoarseGrained::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "motility", m_motility);
		file->writeAttributeDouble(groupName, "epsilon", m_epsilon);
		file->writeAttributeDouble(groupName, "cut", m_cut);
		file->writeAttributeDouble(groupName, "rMax2", m_rMaxSquared);
		file->writeAttributeDouble(groupName, "k", m_kappa);
		file->writeAttributeDouble(groupName, "rateDuplication", m_rateDuplication);
		file->writeAttributeDouble(groupName, "thresholdDuplication", m_thresholdDuplication);

		file->writeAttributeDouble(groupName, "ssCellLength", m_ssCellLength);
		file->writeAttributeDouble(groupName, "ssSpeed", m_ssSpeed);
		file->writeAttributeDouble(groupName, "migrationTime", m_migrationTime);
		file->writeAttributeBool(groupName, "CIL", m_CIL);
		file->writeAttributeBool(groupName, "division", m_division);


		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(MIGRATION).save(file, partTypesGroupName);


		if (m_division) {
			strcpy(partTypesGroupName, groupName);
			strcat(partTypesGroupName, "/Types_2");
			this->partTypes.at(DIVISION).save(file, partTypesGroupName);
		}


		return true;
	}
	catch (H5::Exception &err)
	{
		err.printError(); // TODO: logger
		return false;
	}
}


bool SMTYSpecsCoarseGrained::cellIsBroken(const Cell* cell, const Box* box) const
{
	// in the SMTY system a cell is broken when its length is
	// bigger than Rmax
	return CheckBroken::tooLong(cell, box, m_rMaxSquared);
}

bool SMTYSpecsCoarseGrained::cellIsDead(const Cell* cell, const Box* box)
{
	// in the SMTY system a cell is dead when it's length is
	// bigger than sigAA and it is not in the division stage
	const double& sigAA = partTypes.at(DIVISION).getPartTypes().at(0).sig;
	return CheckDead::cellIsPulledApart(cell, box, sigAA * sigAA, DIVISION);

}

bool SMTYSpecsCoarseGrained::divisionCriterion(Cell* cell, const Box* box) const
{
	const double& sigAA = partTypes.at(DIVISION).getPartTypes().at(0).sig;
	return CheckLength::cellIsLongerThan(1.0 * 1.0 * sigAA * sigAA, cell, box);
}


bool SMTYSpecsCoarseGrained::cellDivides(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
	return DivisionMethods::standardDivision(cell, newCells, box, cellCounter, cycleLength, MIGRATION);
}



bool SMTYSpecsCoarseGrained::endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const
{
	Part* part = &cell->getPart(0);

	if (part->currentStage == DIVISION)
	{
		if (part->currentStageTime == (m_divisionCycleLength - 1))
		{
			// division stage has just terminated. Check if the cell has to be divided
			if (divisionCriterion(cell, box))
				return true;
			else
				return false;
		}

		return false;
	}

	return false;
}


