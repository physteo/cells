#include "SMTYSpecsCycleSoftCoreNOCIL.h"

SMTYSpecsCycleSoftCoreNOCIL::SMTYSpecsCycleSoftCoreNOCIL() : PartSpecs(2, 3)
{
	name = "SMTY-Cycle-SoftCore-NOCIL";
	m_numberOfStages = this->partTypes.size();
}

void SMTYSpecsCycleSoftCoreNOCIL::build()
{
	std::cout << " *** System's steady-state(ss) characteristics *** " << std::endl;
	std::cout << " - Cell Length = " << m_ssCellLength << std::endl;
	std::cout << " - Speed at ss = " << m_ssSpeed << std::endl;
	std::cout << " - Migration t = " << m_migrationTime << std::endl;

	// stage EXTENSION
	{
		const double& sigAA     = this->partTypes.at(EXTENSION).getPartTypes().at(0).sig;
		const double& sigBB     = this->partTypes.at(EXTENSION).getPartTypes().at(1).sig;
		const double& frictionF = this->partTypes.at(EXTENSION).getPartTypes().at(0).friction;
		const double& frictionB = this->partTypes.at(EXTENSION).getPartTypes().at(1).friction;

		double sigAB = .5*(sigAA + sigBB);


		SMSoftCore* scAA(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigAA });
		SMSoftCore* scAB(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigAB });
		SMSoftCore* scBB(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigBB });

		addInterForce(EXTENSION, 0, scAA);
		addInterForce(EXTENSION, 1, scAB);
		addInterForce(EXTENSION, 2, scAB);
		addInterForce(EXTENSION, 3, scBB);

		FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });

		addIntraForce(EXTENSION, 0, feneAA);
		addIntraForce(EXTENSION, 1, feneAB);
		addIntraForce(EXTENSION, 2, feneBA);
		addIntraForce(EXTENSION, 3, feneBB);

		ConstantPropulsionForce* nocil(new ConstantPropulsionForce{ m_motility * m_ssCellLength });
		addIntraForce(EXTENSION, 1, nocil);
		addIntraForce(EXTENSION, 2, nocil);
		
		// noise
		//partSpecs.oneBodyForces.at(0).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7})))); // TODO URGENT: pass dt etc
		//partSpecs.oneBodyForces.at(1).push_back(std::move(std::make_unique<GaussianRandomForce>(std::move(GaussianRandomForce{ 1.0, 0.001,1.0,1.0,4.707646e-7 })))); // TODO URGENT: pass dt etc
	}

	// stage CONTRACTION
	{
		const double& sigAA     = this->partTypes.at(CONTRACTION).getPartTypes().at(0).sig;
		const double& sigBB     = this->partTypes.at(CONTRACTION).getPartTypes().at(1).sig;
		const double& frictionF = this->partTypes.at(CONTRACTION).getPartTypes().at(0).friction;
		const double& frictionB = this->partTypes.at(CONTRACTION).getPartTypes().at(1).friction;

		double sigAB = .5*(sigAA + sigBB);

		SMSoftCore* scAA(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigAA });
		SMSoftCore* scAB(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigAB });
		SMSoftCore* scBB(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigBB });

		addInterForce(CONTRACTION, 0, scAA);
		addInterForce(CONTRACTION, 1, scAB);
		addInterForce(CONTRACTION, 2, scAB);
		addInterForce(CONTRACTION, 3, scBB);

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

	// stage DIVISION
	{
		const double& sigAA = this->partTypes.at(CONTRACTION).getPartTypes().at(0).sig;
		const double& sigBB = this->partTypes.at(CONTRACTION).getPartTypes().at(1).sig;
		const double& frictionF = this->partTypes.at(CONTRACTION).getPartTypes().at(0).friction;
		const double& frictionB = this->partTypes.at(CONTRACTION).getPartTypes().at(1).friction;

		double sigAB = .5*(sigAA + sigBB);


		SMSoftCore* scAA(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigAA });
		SMSoftCore* scAB(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigAB });
		SMSoftCore* scBB(new SMSoftCore{ m_ewell, m_ecore, m_xi, sigBB });

		addInterForce(DIVISION, 0, scAA);
		addInterForce(DIVISION, 1, scAB);
		addInterForce(DIVISION, 2, scAB);
		addInterForce(DIVISION, 3, scBB);

		FeneForce* feneAA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneAB(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBA(new FeneForce{ m_rMaxSquared, m_kappa });
		FeneForce* feneBB(new FeneForce{ m_rMaxSquared, m_kappa });

		addIntraForce(DIVISION, 0, feneAA);
		addIntraForce(DIVISION, 1, feneAB);
		addIntraForce(DIVISION, 2, feneBA);
		addIntraForce(DIVISION, 3, feneBB);

		// no cil force
		ConstantPropulsionForce* nocil(new ConstantPropulsionForce{ 2.0 * m_motility * m_ssCellLength }); // TODO urgent: remove 5.0
		addIntraForce(DIVISION, 1, nocil);
		addIntraForce(DIVISION, 2, nocil);
	}


}


SMTYSpecsCycleSoftCoreNOCIL::SMTYSpecsCycleSoftCoreNOCIL(double sigAA, double sigBB, double motility,
	double ewell, double ecore, double xi, double rMaxSquared, double kappa, double frictionF, double originalFrictionF,
	double frictionB, double originalFrictionB, double massF, double massB,
	double rateDuplication, double thresholdDuplication,
	size_t cycleLength, size_t divisionCycleLength) : SMTYSpecsCycleSoftCoreNOCIL()
{

	m_motility = motility;
	m_ewell = ewell;
	m_ecore = ecore;
	m_xi    = xi;

	m_rMaxSquared          = rMaxSquared;
	m_kappa                = kappa;
	m_cycleLength          = cycleLength;
	m_rateDuplication      = rateDuplication;
	m_thresholdDuplication = thresholdDuplication;
	m_divisionCycleLength = divisionCycleLength;

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
		else{
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


	// stage Division
	{
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


SMTYSpecsCycleSoftCoreNOCIL::SMTYSpecsCycleSoftCoreNOCIL(const Parameters* params, size_t cycleLength, size_t divisionCycleLength) :
	SMTYSpecsCycleSoftCoreNOCIL(params->getParam(0),
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
		params->getParam(15),
		cycleLength, divisionCycleLength)
{
}


bool SMTYSpecsCycleSoftCoreNOCIL::load(Hdf5* file, const char* groupName)
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
		m_ewell = file->readAttributeDouble(groupName, "ewell");
		m_ecore = file->readAttributeDouble(groupName, "ecore");
		m_xi = file->readAttributeDouble(groupName, "xi");
		m_rMaxSquared = file->readAttributeDouble(groupName, "rMax2");
		m_kappa = file->readAttributeDouble(groupName, "k");
		m_rateDuplication = file->readAttributeDouble(groupName, "rateDuplication");
		m_thresholdDuplication = file->readAttributeDouble(groupName, "thresholdDuplication");
		m_cycleLength = file->readAttributeInteger(groupName, "cycleLength");

		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(EXTENSION).load(file, partTypesGroupName);

		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_1");
		this->partTypes.at(CONTRACTION).load(file, partTypesGroupName);

		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_2");
		this->partTypes.at(DIVISION).load(file, partTypesGroupName);


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



bool SMTYSpecsCycleSoftCoreNOCIL::save(Hdf5* file, const char* groupName) const
{
	try {
		file->createNewGroup(groupName);
		H5::Group group = file->openGroup(groupName);

		file->writeAttributeString(groupName, "name", name.c_str());
		file->writeAttributeDouble(groupName, "motility", m_motility);
		file->writeAttributeDouble(groupName, "ewell", m_ewell);
		file->writeAttributeDouble(groupName, "ecore", m_ecore);
		file->writeAttributeDouble(groupName, "xi", m_xi);
		file->writeAttributeDouble(groupName, "rMax2", m_rMaxSquared);
		file->writeAttributeDouble(groupName, "k", m_kappa);
		file->writeAttributeDouble(groupName, "rateDuplication", m_rateDuplication);
		file->writeAttributeDouble(groupName, "thresholdDuplication", m_thresholdDuplication);
		file->writeAttributeInteger(groupName, "cycleLength", m_cycleLength);

		file->writeAttributeDouble(groupName, "ssCellLength", m_ssCellLength);
		file->writeAttributeDouble(groupName, "ssSpeed", m_ssSpeed);
		file->writeAttributeDouble(groupName, "migrationTime", m_migrationTime);


		// save partType
		char partTypesGroupName[64];
		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_0");
		this->partTypes.at(EXTENSION).save(file, partTypesGroupName);

		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_1");
		this->partTypes.at(CONTRACTION).save(file, partTypesGroupName);

		strcpy(partTypesGroupName, groupName);
		strcat(partTypesGroupName, "/Types_2");
		this->partTypes.at(DIVISION).save(file, partTypesGroupName);



		return true;
	}
	catch (H5::Exception &err)
	{
		err.printError(); // TODO: logger
		return false;
	}
}


bool SMTYSpecsCycleSoftCoreNOCIL::cellIsBroken(const Cell* cell, const Box* box) const
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


bool SMTYSpecsCycleSoftCoreNOCIL::cellIsDead(const Cell* cell, const Box* box)
{
	return false;
}

bool SMTYSpecsCycleSoftCoreNOCIL::divisionCriterion(Cell* cell, const Box* box) const
{
	Vector vectorDistance;
	double distance2 = box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, vectorDistance);

	const double& sigAA = partTypes.at(DIVISION).getPartTypes().at(0).sig;

	if (distance2 >=  1.0 * sigAA * sigAA)
	{
		return true;
	}
	else {
		return false;
	}
}

bool SMTYSpecsCycleSoftCoreNOCIL::cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const
{
		// bla
		Cell newcell = Cell(2);
		newcell.getPart(0).type = 0;
		newcell.getPart(0).cell = cellCounter;

		newcell.getPart(0).position = cell->getPart(1).position;

		newcell.getPart(1).type = 1;
		newcell.getPart(1).cell = cellCounter;
		newcell.getPart(1).position = cell->getPart(1).position;


		// set velocities to zero
		newcell.getPart(0).velocity = Vector{ 0.0, 0.0 };
		newcell.getPart(1).velocity = Vector{ 0.0, 0.0 };

		// assign stage
		unsigned short cellStage = gsl_rng_uniform_int(g_rng, cycleLength);
		newcell.getPart(0).stage = cellStage;
		newcell.getPart(1).stage = cellStage;


		newcell.getPart(0).currentStage = EXTENSION;
		newcell.getPart(1).currentStage = EXTENSION;
		newcell.getPart(0).currentStageTime = 0;
		newcell.getPart(1).currentStageTime = 0;


		newCells->push_back(newcell);
		cellCounter++;

		// modify old cell. Do not change its cell ID!
		cell->getPart(1).position = cell->getPart(0).position;
		cell->getPart(0).velocity = Vector{ 0.0,0.0 };
		cell->getPart(1).velocity = Vector{ 0.0,0.0 };
		return true;
		
}



bool SMTYSpecsCycleSoftCoreNOCIL::endOfDivisionStage(Cell* cell, const Box* box) const
{
	Part* part = &cell->getPart(0);

	if (part->currentStage == DIVISION)
	{
		if (part->currentStageTime == (m_divisionCycleLength - 1))
		{
			// division stage has just terminated. Check if the cell has to be duplicated
			if (divisionCriterion(cell, box))
				return true;
			else
				return false;
		}

		return false;
	}

	return false;

}


void SMTYSpecsCycleSoftCoreNOCIL::updateStage(size_t time, Cell* cell) const
{
	size_t currentCycleLength;

	Part* part1 = &cell->getPart(0);
	Part* part2 = &cell->getPart(1);

	if (part1->currentStage == DIVISION)
	{
		currentCycleLength = m_divisionCycleLength;
	}
	else
	{
		currentCycleLength = m_cycleLength;
	}



	if (    part1->currentStageTime == (currentCycleLength - 1)   )
	{
		// update stage
		if (gsl_rng_uniform(g_rng) < m_rateDuplication)
		{
			//std::cout << "a cell is dividing in "<< part1->position.x << "," << part1->position.y << std::endl;
			part1->currentStage = DIVISION;
			part2->currentStage = DIVISION;
		}
		else
		{
			part1->currentStage = (part1->currentStage + 1) % (m_numberOfStages - 1);
			part2->currentStage = (part2->currentStage + 1) % (m_numberOfStages - 1);
		}

		part1->currentStageTime = 0;
		part2->currentStageTime = 0;
	}



}


// new features

