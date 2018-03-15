#include "SpecsTopological.h"


SpecsTopological::SpecsTopological()
{

}


void SpecsTopological::build()
{
	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << m_ssCellLength << std::endl;
	std::cout << " - Speed at ss = " << m_ssSpeed << std::endl;
	std::cout << " - Migration t = " << m_migrationTime << std::endl;


	MorseForce* backBack(new MorseForce{100e-5, 10.0, 2.0});
	//NewLJForce* backBack(new NewLJForce{ m_epsilon, 2.5 * 2.5 });
	
	NewLJForce* frontFront(new NewLJForce{ 10e-6,  pow(2.0, 1.0/6.0) * pow(2.0, 1.0 / 6.0) });
	//NewSMSoftCore* interForce(new NewSMSoftCore{ 0.025, 1.0, 0.1, 1.0 });

	FeneForce* fene(new FeneForce{ m_rMaxSquared, m_kappa });

	// stage MIGRATION
	{
		addInterForce(MIGRATION, 0, frontFront);
		addInterForce(MIGRATION, 1, frontFront);
		addInterForce(MIGRATION, 2, frontFront);
		addInterForce(MIGRATION, 3, backBack);

		addIntraForce(MIGRATION, 0, fene);
		addIntraForce(MIGRATION, 1, fene);
		addIntraForce(MIGRATION, 2, fene);
		addIntraForce(MIGRATION, 3, fene);


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
		addInterForce(DIVISION, 0, frontFront);
		addInterForce(DIVISION, 1, frontFront);
		addInterForce(DIVISION, 2, frontFront);
		addInterForce(DIVISION, 3, frontFront);


		addIntraForce(DIVISION, 0, fene);
		addIntraForce(DIVISION, 1, fene);
		addIntraForce(DIVISION, 2, fene);
		addIntraForce(DIVISION, 3, fene);

		// propulsion force to both parts
		//		ConstantPropulsionForce* nocil(new ConstantPropulsionForce{ 1.0 * m_motility * m_ssCellLength }); // TODO urgent: remove hard coded value 2.0
		//		addIntraForce(DIVISION, 1, nocil);
		//		addIntraForce(DIVISION, 2, nocil);

		CilForce*  cil(new CilForce{ m_motility });
		addIntraForce(DIVISION, 1, cil);
		addIntraForce(DIVISION, 2, cil);

		// cycle manager
		cycleManager = new SingleStageWithDivisionImproved{ m_divisionCycleLength, m_rateDuplication, DIVISION, &partTypes.at(DIVISION) };

	}
	else {
		cycleManager = new SingleStage{};
	}


}


SpecsTopological::SpecsTopological(double sigAA, double sigBB, double motility,
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


SpecsTopological::SpecsTopological(const Parameters* params, size_t divisionCycleLength, bool CIL, bool division) :
	SpecsTopological(params->getParam(0),
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


bool SpecsTopological::load(Hdf5* file, const char* groupName)
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



bool SpecsTopological::save(Hdf5* file, const char* groupName) const
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


bool SpecsTopological::cellIsBroken(const Cell* cell, const Box* box) const
{
	// in the SMTY system a cell is broken when its length is
	// bigger than Rmax
	return CheckBroken::tooLong(cell, box, m_rMaxSquared);
}

bool SpecsTopological::cellIsDead(const Cell* cell, const Box* box)
{
	// in the SMTY system a cell is dead when it's length is
	// bigger than sigAA and it is not in the division stage
	const double& sigAA = partTypes.at(DIVISION).getPartTypes().at(0).sig;
	return CheckDead::cellIsPulledApart(cell, box, sigAA * sigAA, DIVISION);
}

bool SpecsTopological::divisionCriterion(Cell* cell, const Box* box) const
{
	const double& sigAA = partTypes.at(DIVISION).getPartTypes().at(0).sig;
	return CheckLength::cellIsLongerThan(1.0 * 1.0 * sigAA * sigAA, cell, box);
}


bool SpecsTopological::cellDivides(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
	return DivisionMethods::standardDivisionWithRandomDisplacement(cell, newCells, box, cellCounter, cycleLength, MIGRATION, &this->partTypes.at(0));
}



bool SpecsTopological::endOfSuccessfullDivisionStage(Cell* cell, const Box* box) const
{
	Part* part = &cell->getPart(0);

	if (part->currentStage == DIVISION)
	{
		if (part->currentStageTime == (m_divisionCycleLength))
		{
			// division stage has just terminated. Check if the cell has to be divided
			if (divisionCriterion(cell, box))
				return true;
			else
			{
				// debug
				//Vector pos1 = cell->getPart(0).position;
				//Vector pos2 = cell->getPart(1).position;
				//
				//Vector distance = pos1 - pos2;
				//
				//std::cout << "Failed. Distance: " << sqrt(Vector::dotProduct(distance, distance)) << std::endl;

				// TODO: URGENT: remove this hack. Do that cyclemanager->magage also tells if the division is successfull or not and does the 
				// proper decisions
				cell->getPart(1).position = cell->getPart(0).position;
				cell->getPart(1).position += Vector{ gsl_rng_uniform(g_rng), gsl_rng_uniform(g_rng) } *0.01;

				// this is correct:
				return false;
			}
		}

		return false;
	}

	return false;
}


