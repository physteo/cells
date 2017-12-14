#include "SMTYSpecsCycleLJ.h"


SMTYSpecsCycleLJ::SMTYSpecsCycleLJ()
{

}


void SMTYSpecsCycleLJ::build()
{
	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << m_ssCellLength << std::endl;
	std::cout << " - Speed at ss = " << m_ssSpeed << std::endl;
	std::cout << " - Migration t = " << m_migrationTime << std::endl;

	// stage EXTENSION
	{
		const double& sigAA = this->partTypes.at(EXTENSION).getPartTypes().at(0).sig;
		const double& sigBB = this->partTypes.at(EXTENSION).getPartTypes().at(1).sig;
		const double& frictionF = this->partTypes.at(EXTENSION).getPartTypes().at(0).friction;
		const double& frictionB = this->partTypes.at(EXTENSION).getPartTypes().at(1).friction;

		double sigAB = .5*(sigAA + sigBB);


		LJForce* lja(new LJForce{ sigAA, m_epsilon, (sigAA *   sigAA * m_cut * m_cut) });
		LJForce* ljb(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljc(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljd(new LJForce{ sigBB, m_epsilon, (sigBB *   sigBB * m_cut * m_cut) });

		addInterForce(EXTENSION, 0, lja);
		addInterForce(EXTENSION, 1, ljb);
		addInterForce(EXTENSION, 2, ljc);
		addInterForce(EXTENSION, 3, ljd);

		FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });

		addIntraForce(EXTENSION, 0, feneAA);
		addIntraForce(EXTENSION, 1, feneAB);
		addIntraForce(EXTENSION, 2, feneBA);
		addIntraForce(EXTENSION, 3, feneBB);


		if (m_CIL == false) {
			ConstantPropulsionForce* nocil(new ConstantPropulsionForce{ m_motility * m_ssCellLength });
			addIntraForce(EXTENSION, 1, nocil);
			addIntraForce(EXTENSION, 2, nocil);
		}
		else if (m_CIL == true)
		{
			CilForce*  cil(new CilForce{ m_motility });
			addIntraForce(EXTENSION, 1, cil);
			addIntraForce(EXTENSION, 2, cil);
		}
		// noise
		//partSpecs.oneBodyForces.at(0).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7})))); // TODO URGENT: pass dt etc
		//partSpecs.oneBodyForces.at(1).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7 })))); // TODO URGENT: pass dt etc
	}

	// stage CONTRACTION
	{
		const double& sigAA = this->partTypes.at(CONTRACTION).getPartTypes().at(0).sig;
		const double& sigBB = this->partTypes.at(CONTRACTION).getPartTypes().at(1).sig;
		const double& frictionF = this->partTypes.at(CONTRACTION).getPartTypes().at(0).friction;
		const double& frictionB = this->partTypes.at(CONTRACTION).getPartTypes().at(1).friction;

		double sigAB = .5*(sigAA + sigBB);

		LJForce* lja(new LJForce{ sigAA, m_epsilon, (sigAA *   sigAA * m_cut * m_cut) });
		LJForce* ljb(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljc(new LJForce{ sigAB, m_epsilon, (sigAB *   sigAB * m_cut * m_cut) });
		LJForce* ljd(new LJForce{ sigBB, m_epsilon, (sigBB *   sigBB * m_cut * m_cut) });

		addInterForce(CONTRACTION, 0, lja);
		addInterForce(CONTRACTION, 1, ljb);
		addInterForce(CONTRACTION, 2, ljc);
		addInterForce(CONTRACTION, 3, ljd);

		FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });

		addIntraForce(CONTRACTION, 0, feneAA);
		addIntraForce(CONTRACTION, 1, feneAB);
		addIntraForce(CONTRACTION, 2, feneBA);
		addIntraForce(CONTRACTION, 3, feneBB);

		// no motility force!

		// noise
		//partSpecs.oneBodyForces.at(0).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7})))); // TODO URGENT: pass dt etc
		//partSpecs.oneBodyForces.at(1).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7 })))); // TODO URGENT: pass dt etc
	}


	if (m_division) {
		// stage DIVISION

		const double& sigAA = this->partTypes.at(DIVISION).getPartTypes().at(0).sig;
		const double& sigBB = this->partTypes.at(DIVISION).getPartTypes().at(1).sig;
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
		cycleManager = new ExtensionContractionDivision{ m_cycleLength, m_divisionCycleLength, m_rateDuplication, DIVISION };

	}
	else {
		cycleManager = new ExtensionContraction{ m_cycleLength };
	}

	std::cout << this << std::endl;

}


SMTYSpecsCycleLJ::SMTYSpecsCycleLJ(double sigAA, double sigBB, double motility,
	double epsilon, double cut, double rMaxSquared, double kappa, double frictionF, double originalFrictionF,
	double frictionB, double originalFrictionB, double massF, double massB,
	double rateDuplication, double thresholdDuplication,
	size_t cycleLength, size_t divisionCycleLength, bool CIL, bool division)
{

	m_division = division;
	m_numberOfStages = division ? 3 : 2;

	this->init(2, m_numberOfStages, specsName);

	std::cout << "build name: " << this->name << std::endl;

	m_motility = motility;
	m_epsilon = epsilon;
	m_cut = cut;

	m_rMaxSquared = rMaxSquared;
	m_kappa = kappa;
	m_cycleLength = cycleLength;
	m_rateDuplication = rateDuplication;
	m_thresholdDuplication = thresholdDuplication;
	m_divisionCycleLength = divisionCycleLength;

	m_CIL = CIL;


	// stage Extension
	{
		this->partTypes.at(EXTENSION).getPartTypes().at(0).name = "F";
		this->partTypes.at(EXTENSION).getPartTypes().at(1).name = "B";
		this->partTypes.at(EXTENSION).getPartTypes().at(0).mass = massF;
		this->partTypes.at(EXTENSION).getPartTypes().at(1).mass = massB;
		this->partTypes.at(EXTENSION).getPartTypes().at(0).sig = sigAA;
		this->partTypes.at(EXTENSION).getPartTypes().at(1).sig = sigBB;



		this->partTypes.at(EXTENSION).getPartTypes().at(0).friction = originalFrictionF;

		if (frictionB <= 0) {
			this->partTypes.at(EXTENSION).getPartTypes().at(1).friction = std::numeric_limits<double>::infinity();
		}
		else {
			this->partTypes.at(EXTENSION).getPartTypes().at(1).friction = frictionB;
		}
	}

	// stage Contraction
	{
		this->partTypes.at(CONTRACTION).getPartTypes().at(0).name = "F";
		this->partTypes.at(CONTRACTION).getPartTypes().at(1).name = "B";
		this->partTypes.at(CONTRACTION).getPartTypes().at(0).mass = massF;
		this->partTypes.at(CONTRACTION).getPartTypes().at(1).mass = massB;
		this->partTypes.at(CONTRACTION).getPartTypes().at(0).sig = sigAA;
		this->partTypes.at(CONTRACTION).getPartTypes().at(1).sig = sigBB;

		if (frictionF <= 0) {
			this->partTypes.at(CONTRACTION).getPartTypes().at(0).friction = std::numeric_limits<double>::infinity();
		}
		else {
			this->partTypes.at(CONTRACTION).getPartTypes().at(0).friction = frictionF;
		}

		this->partTypes.at(CONTRACTION).getPartTypes().at(1).friction = originalFrictionB;
	}

	if (m_division) {

		this->partTypes.at(DIVISION).getPartTypes().at(0).name = "F";
		this->partTypes.at(DIVISION).getPartTypes().at(1).name = "B";
		this->partTypes.at(DIVISION).getPartTypes().at(0).mass = massF;
		this->partTypes.at(DIVISION).getPartTypes().at(1).mass = massB;
		this->partTypes.at(DIVISION).getPartTypes().at(0).sig = sigAA;
		this->partTypes.at(DIVISION).getPartTypes().at(1).sig = sigBB;
		this->partTypes.at(DIVISION).getPartTypes().at(0).friction = originalFrictionF;
		this->partTypes.at(DIVISION).getPartTypes().at(1).friction = originalFrictionB;
	}


	// characteristics
	m_ssCellLength = sqrt(m_rMaxSquared) * sqrt(1.0 - m_kappa * (1.0 / originalFrictionF + 1.0 / originalFrictionB) / (m_motility / originalFrictionF));
	m_ssSpeed = m_ssCellLength * m_motility / (originalFrictionF + originalFrictionB);
	m_migrationTime = sqrt(m_rMaxSquared) / m_ssSpeed;


	build();
}


SMTYSpecsCycleLJ::SMTYSpecsCycleLJ(const Parameters* params, size_t cycleLength, size_t divisionCycleLength, bool CIL, bool division) :
	SMTYSpecsCycleLJ(params->getParam(0),
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
		params->getParam(13),
		params->getParam(14),
		cycleLength, divisionCycleLength, CIL, division)
{
}


bool SMTYSpecsCycleLJ::load(Hdf5* file, const char* groupName)
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
		m_cycleLength = file->readAttributeInteger(groupName, "cycleLength");
		m_CIL = file->readAttributeBool(groupName, "CIL");
		m_division = file->readAttributeBool(groupName, "division");

		this->init(2, m_division ? 3 : 2, specsName);

		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(EXTENSION).load(file, partTypesGroupName);

		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_1");
		this->partTypes.at(CONTRACTION).load(file, partTypesGroupName);

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



bool SMTYSpecsCycleLJ::save(Hdf5* file, const char* groupName) const
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
		file->writeAttributeInteger(groupName, "cycleLength", m_cycleLength);

		file->writeAttributeDouble(groupName, "ssCellLength", m_ssCellLength);
		file->writeAttributeDouble(groupName, "ssSpeed", m_ssSpeed);
		file->writeAttributeDouble(groupName, "migrationTime", m_migrationTime);
		file->writeAttributeBool(groupName, "CIL", m_CIL);
		file->writeAttributeBool(groupName, "division", m_division);


		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(EXTENSION).save(file, partTypesGroupName);

		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_1");
		this->partTypes.at(CONTRACTION).save(file, partTypesGroupName);

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


bool SMTYSpecsCycleLJ::cellIsBroken(const Cell* cell, const Box* box) const
{
	// in the SMTY system a cell is broken when its length is
	// bigger than Rmax
	return CheckBroken::tooLong(cell, box, m_rMaxSquared);
}

bool SMTYSpecsCycleLJ::cellIsDead(const Cell* cell, const Box* box)
{
	// in the SMTY system a cell is dead when it's length is
	// bigger than sigAA and it is not in the division stage
	const double& sigAA = partTypes.at(DIVISION).getPartTypes().at(0).sig;
	return CheckDead::cellIsPulledApart(cell, box, sigAA * sigAA, DIVISION);

}

bool SMTYSpecsCycleLJ::divisionCriterion(Cell* cell, const Box* box) const
{
	const double& sigAA = partTypes.at(DIVISION).getPartTypes().at(0).sig;
	return CheckLength::cellIsLongerThan(sigAA * sigAA, cell, box);
}


bool SMTYSpecsCycleLJ::cellDivides(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
	return DivisionMethods::standardDivision(cell, newCells, box, cellCounter, cycleLength, EXTENSION);
}



bool SMTYSpecsCycleLJ::endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const
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




// new features

