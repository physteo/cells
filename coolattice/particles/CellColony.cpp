#include "CellColony.h"

size_t CellColony::totalNumberOfParts() const
{
	size_t tot = 0;
	for(size_t i = 0; i < size(); i++)
		tot += m_cells.at(i).getNumOfParts();
	return tot;
}

void CellColony::addVelocity(double vx, double vy, size_t cell, size_t  type)
{
	this->m_cells.at(cell).getPart(type).velocity += Vector{vx, vy};
}

void CellColony::addOnePartCell(double x, double y, double vx, double vy, unsigned short stage, size_t currentStageTime, double sig)
{
	size_t cell = m_cells.size();
	Part p{ Vector{ x, y }, Vector{ vx, vy }, 0, cell, stage, currentStageTime, sig };
	std::vector<Part> parts = { p };
#ifdef OBJECTPOOL
	this->m_cells.addBackElement(parts);
	
	//this->m_cells.addBackElement();
	//m_cells.back().init(parts);
#else
	Cell newCell{ parts };
	this->m_cells.push_back(newCell);
#endif
}

void CellColony::addTwoPartsCell(double x1, double y1, double vx1, double vy1,
	double x2, double y2, double vx2, double vy2, unsigned short stage, size_t currentStageTime, double sigA, double sigB)
{
	size_t cell = m_cells.size();
	Part p1{ Vector{ x1, y1 }, Vector{ vx1, vy1 }, 0, cell, stage, currentStageTime, sigA };
	Part p2{ Vector{ x2, y2 }, Vector{ vx2, vy2 }, 1, cell, stage, currentStageTime, sigB };

	std::vector<Part> parts = { p1, p2 };

#ifdef OBJECTPOOL
	this->m_cells.addBackElement(parts);
	//this->m_cells.addBackElement();
	//m_cells.back().init(parts);
#else
	Cell newCell{ parts };
	this->m_cells.push_back(newCell);
#endif
}


void CellColony::assignSingleCellStageAndTime(int cellID, unsigned short currentStageTime, unsigned short currentStage)
{
	m_cells.at(cellID).getPart(0).currentStage = currentStage;
	m_cells.at(cellID).getPart(0).currentStageTime = currentStageTime;

	m_cells.at(cellID).getPart(1).currentStage = currentStage;
	m_cells.at(cellID).getPart(1).currentStageTime = currentStageTime;


}


void CellColony::assignCycleStageTime(unsigned short cycleLength, unsigned short numberOfStages)
{
	for (size_t i = 0; i < m_cells.size(); i++)
	{
		unsigned short currentStage     = gsl_rng_uniform_int(g_rng, numberOfStages);
		unsigned short currentStageTime = gsl_rng_uniform_int(g_rng, cycleLength);
		for (size_t j = 0; j < m_cells.at(i).getNumOfParts(); j++)
		{
			m_cells.at(i).getPart(j).currentStage     = currentStage;
			m_cells.at(i).getPart(j).currentStageTime = currentStageTime;
		}
	}
}

void CellColony::populateLane(double numPerLineX, double numPerLineY, double boxLengthX, double boxLengthY, double sigA, double sigB, double offsetx, double offsety)
{
	this->populate(numPerLineX, numPerLineY, boxLengthX, boxLengthY, sigA, sigB);
	
// apply offset
	for (size_t i = 0; i < m_cells.size(); i++)
	{
		Cell& c = this->m_cells.at(i);
		for (size_t j = 0; j < c.getNumOfParts(); j++)
		{
			Part& p = c.getPart(j);
			p.position.x += offsetx;
			p.position.y += offsety;
		}

	}
}



// N is the total number of particles we want. The resulting number of particles will be close to that number.
// f is the slab dimension, as a submultiple of the boxLengthX 
void CellColony::populateSlab(double N, double f, double boxLengthX, double boxLengthY, double sigA, double sigB)
{
	double numPerLineX = static_cast<int>(sqrt(f * N));
	double numPerLineY = static_cast<int>(sqrt(N / f));

	double lx = f * boxLengthX / numPerLineX;
	double ly =     boxLengthY / numPerLineY; // they're the same if boxLengthX == boxLengthY

	double startX = 0.5 * (1.0 - f) * boxLengthX;
	double startY = 0.0;

	double sigMax = std::max(sigA, sigB);
	double sigMin = std::min(sigA, sigB);

	size_t numberOfCells = 0;
	for (int x = 0; x < numPerLineX; x++)
	{
		for (int y = 0; y < numPerLineY; y++)
		{
			//std::vector<Part> cell;
			Cell cell = Cell(2);
			Vector positionF{ startX + (x + 0.5) * lx, startY + (y + 0.5) * ly };
			
			Vector positionB = positionF +
				Vector{ (2.0 * gsl_rng_uniform(g_rng) - 1.0),
						(2.0 * gsl_rng_uniform(g_rng) - 1.0) } * (sigMax - sigMin) * 0.1;

			// check if the distance is bigger than rmax.. or better sigMin so i dont have to pass more stuff inside
			Vector distanceVector = positionB - positionF;
			double distance2 = Vector::dotProduct(distanceVector, distanceVector);

			if (distance2 > sigMin*sigMin)
			{
				positionB = positionF;
			}
			cell.getPart(0).position = positionF;
			cell.getPart(1).position = positionB;
			cell.getPart(0).velocity = Vector{0.0,0.0};
			cell.getPart(1).velocity = Vector{0.0,0.0};
			cell.getPart(0).type = 0;
			cell.getPart(1).type = 1;

			cell.getPart(0).currentStage     = 0;
			cell.getPart(1).currentStage     = 0;
			cell.getPart(0).currentStageTime = 0;
			cell.getPart(1).currentStageTime = 0;
			cell.getPart(0).currentSigma = sigA;
			cell.getPart(1).currentSigma = sigB;

			cell.getPart(0).cell = numberOfCells;
			cell.getPart(1).cell = numberOfCells;


			this->push_back(cell);
			numberOfCells++;
		}
	}
	std::cout << "Number of cells: " << this->size() << std::endl;
}


void CellColony::setPolydispersity(double sigMax, double sigMin, int species, double ratioBF, double accuracy)
{
	bool goodToGo = false;


	while (!goodToGo)
	{

		if (sigMax < sigMin)
		{
			double tmp = sigMin;
			sigMin = sigMax;
			sigMax = tmp;
		}

		std::vector<double> sizesF;
		std::vector<size_t> populations;

		double sigInterval = (sigMax - sigMin) / (double)(species - 1);

		for (int i = 0; i < species; i++)
		{
			sizesF.push_back(sigMin + sigInterval * i);
			populations.push_back(0);
		}

		for (size_t i = 0; i < m_cells.size(); i++)
		{
			Part& front = m_cells.at(i).getPart(0);
			Part& back = m_cells.at(i).getPart(1);


			// assign randomly the sigma
			unsigned short index = gsl_rng_uniform_int(g_rng, species);
			populations.at(index)++;

			front.currentSigma = sizesF.at(index);
			back.currentSigma = ratioBF * front.currentSigma;
		}

		goodToGo = true;

		// check quality of randomization
		size_t numberOfCells = m_cells.size();
		double perfectRatio = 1.0 / ((double)species);
		for (size_t i = 0; i < populations.size(); i++)
		{
			double value = std::abs( populations.at(i) / ((double)numberOfCells) - perfectRatio);
			if (value > accuracy)
			{
				goodToGo = false;
			}
		}

	}
}


void CellColony::populateDirected(double numPerLineX, double numPerLineY, double boxLengthX, double boxLengthY, double sigA, double sigB)
{
	double lx = boxLengthX / numPerLineX;
	double ly = boxLengthY / numPerLineY;

	double sigMin = std::min(sigA, sigB);

	this->populate(numPerLineX, numPerLineY, boxLengthX, boxLengthY, sigA, sigB);

	// put all back disks on the left side of the front disk
	for (size_t c = 0; c < this->size(); c++)
	{

		Cell& cell = this->at(c);
		Vector& positionF = cell.getPart(0).position;
		Vector& positionB = cell.getPart(1).position;
		Vector displacement = positionF - positionB;
		positionB.x = positionF.x - std::abs(displacement.x);
	}
}

void CellColony::populate(double numPerLineX, double numPerLineY, double boxLengthX, double boxLengthY, double sigA, double sigB)
{
	double lx = boxLengthX / numPerLineX;
	double ly = boxLengthY / numPerLineY;

	double sigMin = std::min(sigA, sigB);

	size_t numberOfCells = 0;
	for (int x = 0; x < numPerLineX; x++)
	{
		for (int y = 0; y < numPerLineY; y++)
		{
			Cell cell = Cell(2);

			Vector positionF{ (x + 0.5)*lx, (y + 0.5)*ly };
			Vector positionB = positionF +
				Vector{ (lx - sigMin) * (2.0 * gsl_rng_uniform(g_rng) - 1.0),
				(ly - sigMin) * (2.0 * gsl_rng_uniform(g_rng) - 1.0) } * 0.5;
			// check if the distance is bigger than rmax.. or better sigMin so i dont have to pass more stuff inside
			Vector distanceVector = positionB - positionF;
			double distance2 = Vector::dotProduct(distanceVector, distanceVector);

			if (distance2 > sigMin*sigMin)
			{
				positionB = positionF;
			}

			cell.getPart(0).position = positionF;
			cell.getPart(1).position = positionB;
			cell.getPart(0).velocity = Vector{ 0.0,0.0 };
			cell.getPart(1).velocity = Vector{ 0.0,0.0 };
			cell.getPart(0).type = 0;
			cell.getPart(1).type = 1;

			cell.getPart(0).currentStageTime = 0;
			cell.getPart(1).currentStageTime = 0;
			cell.getPart(0).currentStage     = 0;
			cell.getPart(1).currentStage     = 0;
			cell.getPart(0).currentSigma = sigA;
			cell.getPart(1).currentSigma = sigB;


			cell.getPart(0).cell = numberOfCells;
			cell.getPart(1).cell = numberOfCells;

			this->push_back(cell);
			numberOfCells++;
		}
	}
	std::cout << "Number of cells: " << this->size() << std::endl;
}



bool CellColony::save(Hdf5* file, const char* name) const
{

	file->createNewGroup(name);
	H5::Group groupCells = file->openGroup(name);

	const H5::CompType& partType = Hdf5types::getPartTypewCell();
	size_t LENGTH = this->totalNumberOfParts();
	int RANK = 1;
	hsize_t dim[] = { LENGTH };   /* Dataspace dimensions */
	H5::DataSpace space(RANK, dim);
	H5::DataSet* dataset = new H5::DataSet(groupCells.createDataSet("c", partType, space));
	std::vector<LightPartwCell> parts;
	parts.resize(LENGTH);
	size_t partCounter = 0;
	for (size_t i = 0; i < m_cells.size(); i++) {
		for (size_t j = 0; j < m_cells.at(i).getNumOfParts(); j++) {
			const Part* part = &m_cells.at(i).getPart(j);
			parts.at(partCounter) = LightPartwCell{ part->position.x, part->position.y, part->velocity.x, part->velocity.y, part->getCell(), part->type, part->currentStage, part->currentStageTime, part->currentSigma };
			partCounter++;
		}
	}
	
	dataset->write(&parts.at(0), partType);
	delete dataset;

	return true;
}

// simon's loading
/*
bool CellColony::load(Hdf5* file, const char* name)
{
	const H5::CompType& simonType = Hdf5types::getSimonType();
	m_cells.resize(0);
	H5::Group groupCells = file->openGroup("snapshots/t_0");

	// positions
	std::vector<Vector> positions;
	{
		H5::DataSet dataset = groupCells.openDataSet("R");
		H5::DataSpace dataspace = dataset.getSpace();
		hsize_t LENGTH = dataspace.getSimpleExtentNpoints();
		positions.resize(LENGTH/2);
		dataset.read(&positions.at(0), H5::PredType::NATIVE_DOUBLE);
	}

	// velocities
	std::vector<Vector> velocities;
	{
		H5::DataSet dataset = groupCells.openDataSet("V");
		H5::DataSpace dataspace = dataset.getSpace();
		hsize_t LENGTH = dataspace.getSimpleExtentNpoints();
		velocities.resize(LENGTH/2);
		dataset.read(&velocities.at(0), H5::PredType::NATIVE_DOUBLE);
	}
	
	std::vector<int> cells;
	{
		H5::DataSet dataset = groupCells.openDataSet("cell");
		H5::DataSpace dataspace = dataset.getSpace();
		hsize_t LENGTH = dataspace.getSimpleExtentNpoints();
		cells.resize(LENGTH);
		dataset.read(&cells.at(0), H5::PredType::NATIVE_INT);
	}
	
	std::vector<int> id;
	{
		H5::DataSet dataset = groupCells.openDataSet("id");
		H5::DataSpace dataspace = dataset.getSpace();
		hsize_t LENGTH = dataspace.getSimpleExtentNpoints();
		id.resize(LENGTH);
		dataset.read(&id.at(0), H5::PredType::NATIVE_INT);
	}


	// fill colony
	std::vector<Part> partsInCell;
	if(positions.size() != velocities.size()) exit(1);
	
	for (size_t i = 0; i < positions.size() - 1; i+=2)
	{
		partsInCell.resize(0);
		Part partB{ Vector{ positions.at(i).x,   positions.at(i).y }, Vector{ velocities.at(i).x , velocities.at(i+1).y },
			(size_t)1, (size_t) cells.at(i) };
	
		Part partF{ Vector{ positions.at(i+1).x, positions.at(i+1).y }, Vector{ velocities.at(i+1).x , velocities.at(i+1).y} ,
			(size_t)0, (size_t) cells.at(i) };
	
		//partF.position *= 1.0 / 0.04;
		//partB.position *= 1.0 / 0.04;
		//
		//partF.velocity *= 1.0 / 0.04;
		//partB.velocity *= 1.0 / 0.04;

		partsInCell.push_back(partF);
		partsInCell.push_back(partB);
	
		m_cells.push_back(partsInCell);
	
	}
	return true;
}
*/

bool CellColony::load(Hdf5* file, const char* name) 
{
	const H5::CompType& partType = Hdf5types::getPartTypewCell();

	// TODO: check that the file exists: why it does not work???
	//if (!file->exists(fileName))
	//	return false;
#ifdef OBJECTPOOL
#else
	m_cells.resize(0);
#endif
	
	H5::Group groupCells = file->openGroup(name);
	H5::DataSet dataset = groupCells.openDataSet("c");
	H5::DataSpace dataspace = dataset.getSpace();
	size_t LENGTH = dataspace.getSimpleExtentNpoints();
	std::vector<LightPartwCell> lightParts;
	lightParts.resize(LENGTH);
	dataset.read(&lightParts.at(0), partType);
	

#ifdef OBJECTPOOL
	m_cells.reserve(LENGTH);
	//m_cells.resize(LENGTH);
#else
#endif

	size_t currentCell = lightParts.at(0).cell;
	std::vector<Part> partsInCell;
	for (size_t i = 0; i < LENGTH; i++) {
		if (!(lightParts.at(i).cell == currentCell))
		{
			if (i + 1 < LENGTH) {
				currentCell = lightParts.at(i + 1).cell;
			}

#ifdef OBJECTPOOL
			this->m_cells.addBackElement(partsInCell);
			
			//this->m_cells.addBackElement();
			//m_cells.back().init(partsInCell);
#else
			m_cells.push_back(partsInCell);
#endif
			partsInCell.resize(0);
		}
		Part part{ Vector{lightParts.at(i).x, lightParts.at(i).y}, Vector{ lightParts.at(i).vx , lightParts.at(i).vy },
			lightParts.at(i).type , lightParts.at(i).cell, lightParts.at(i).currentStage,
			lightParts.at(i).currentStageTime, lightParts.at(i).currentSigma };
		
		
		partsInCell.push_back(part);
	}
		
#ifdef OBJECTPOOL
	this->m_cells.addBackElement(partsInCell);

	//this->m_cells.addBackElement();
	//m_cells.back().init(partsInCell);
#else
	m_cells.push_back(partsInCell);
#endif

	return true;
}




void CellColony::push_back(const Cell& cc)
{
#ifdef OBJECTPOOL
	this->m_cells.addBackElement(cc);

	//this->m_cells.addBackElement();
	//m_cells.back() = cc;
#else
	m_cells.push_back(cc);
#endif
}

Cell& CellColony::at(size_t i)
{
	return m_cells.at(i);
}

const Cell& CellColony::at(size_t i) const
{
	return m_cells.at(i);
}

Cell&	CellColony::back()
{
	return m_cells.back();
}

const Cell	CellColony::back() const
{
	return m_cells.back();
}


void CellColony::reserve(size_t i)
{
#ifdef OBJECTPOOL
	m_cells.reserve(i); // TODO URGENT: see if i can define a suitable reserve function
	//m_cells.resize(i);
#else
	m_cells.reserve(i);
#endif
}


void CellColony::erase(size_t i)
{
#ifdef OBJECTPOOL
	m_cells.deleteElement(i);
#else
	std::cout << "erase feature for vector of cells is not implemented yet." << std::endl;
	std::cin.get();
	//m_cells.erase(i);
#endif
}
