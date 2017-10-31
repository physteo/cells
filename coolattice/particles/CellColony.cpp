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

void CellColony::addOnePartCell(double x, double y, double vx, double vy)
{
	size_t cell = m_cells.size();
	Part p{ Vector{ x, y }, Vector{ vx, vy }, 0, cell };
	std::vector<Part> parts = { p };
#ifdef OBJECTPOOL
	this->m_cells.addBackElement();
	m_cells.back().init(parts);
#else
	Cell newCell{ parts };
	this->m_cells.push_back(newCell);
#endif
}

void CellColony::addTwoPartsCell(double x1, double y1, double vx1, double vy1,
	double x2, double y2, double vx2, double vy2)
{
	size_t cell = m_cells.size();
	Part p1{ Vector{ x1, y1 }, Vector{ vx1, vy1 }, 0, cell };
	Part p2{ Vector{ x2, y2 }, Vector{ vx2, vy2 }, 1, cell };

	std::vector<Part> parts = { p1, p2 };

#ifdef OBJECTPOOL
	this->m_cells.addBackElement();
	m_cells.back().init(parts);
#else
	Cell newCell{ parts };
	this->m_cells.push_back(newCell);
#endif
}

// N is the total number of particles we want. The resulting number of particles will be close to that number.
// f is the slab dimension, as a submultiple of the boxLengthX 
void CellColony::populateSlab(double N, double f, double boxLengthX, double boxLengthY, double sigMin)
{
	double numPerLineX = static_cast<int>(sqrt(f * N));
	double numPerLineY = static_cast<int>(sqrt(N / f));

	double lx = f * boxLengthX / numPerLineX;
	double ly =     boxLengthY / numPerLineY; // they're the same if boxLengthX == boxLengthY

	double startX = 0.5 * (1.0 - f) * boxLengthX;
	double startY = 0.0;

	size_t numberOfCells = 0;
	for (int x = 0; x < numPerLineX; x++)
	{
		for (int y = 0; y < numPerLineY; y++)
		{
			std::vector<Part> cell;
			Vector positionF{ startX + (x + 0.5) * lx, startY + (y + 0.5) * ly };
			
			Vector positionB = positionF +
				Vector{ (lx - sigMin) * (2.0 * gsl_rng_uniform(g_rng) - 1.0),
				(ly - sigMin) * (2.0 * gsl_rng_uniform(g_rng) - 1.0) } *0.5;
			// check if the distance is bigger than rmax.. or better sigMin so i dont have to pass more stuff inside
			Vector distanceVector = positionB - positionF;
			double distance2 = Vector::dotProduct(distanceVector, distanceVector);

			if (distance2 > sigMin*sigMin)
			{
				positionB = positionF;
			}

			cell.push_back(Part{ positionF, Vector{ 0.0, 0.0 },0,numberOfCells });
			cell.push_back(Part{ positionB, Vector{ 0.0, 0.0 },1,numberOfCells });
			this->push_back(cell);
			numberOfCells++;
		}
	}
	std::cout << "Number of cells: " << this->size() << std::endl;
}

void CellColony::populate(double numPerLineX, double numPerLineY, double boxLengthX, double boxLengthY, double sigMin)
{
	double lx = boxLengthX / numPerLineX;
	double ly = boxLengthY / numPerLineY;

	size_t numberOfCells = 0;
	for (int x = 0; x < numPerLineX; x++)
	{
		for (int y = 0; y < numPerLineY; y++)
		{
			std::vector<Part> cell;
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

			cell.push_back(Part{ positionF, Vector{ 0.0, 0.0 },0,numberOfCells });
			cell.push_back(Part{ positionB, Vector{ 0.0, 0.0 },1,numberOfCells });
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
			parts.at(partCounter) = LightPartwCell{ part->position.x, part->position.y, part->velocity.x, part->velocity.y, part->cell, part->type };
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
	m_cells.resize(LENGTH);
#else
#endif

	size_t currentCell = 0;
	std::vector<Part> partsInCell;
	for (size_t i = 0; i < LENGTH; i++) {
		if (!(lightParts.at(i).cell == currentCell))
		{
			currentCell++;
#ifdef OBJECTPOOL
			this->m_cells.addBackElement();
			m_cells.back().init(partsInCell);
#else
			m_cells.push_back(partsInCell);
#endif
			partsInCell.resize(0);
		}
		Part part{ Vector{lightParts.at(i).x, lightParts.at(i).y}, Vector{ lightParts.at(i).vx , lightParts.at(i).vy },
			lightParts.at(i).type , lightParts.at(i).cell };
		partsInCell.push_back(part);
	}
		
#ifdef OBJECTPOOL
	this->m_cells.addBackElement();
	m_cells.back().init(partsInCell);
#else
	m_cells.push_back(partsInCell);
#endif


	return true;
}




void CellColony::push_back(const Cell& cc)
{
#ifdef OBJECTPOOL
	this->m_cells.addBackElement();
	m_cells.back() = cc;
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

void CellColony::resize(size_t i)
{
#ifdef OBJECTPOOL
	//std::cout << "trying to resize cellColony" << std::endl;
	//std::cin.get();
	m_cells.resize(i);
#else
	m_cells.resize(i);
#endif
}

void CellColony::reserve(size_t i)
{
#ifdef OBJECTPOOL
	m_cells.resize(i); // TODO URGENT: see if i can define a suitable reserve function
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
