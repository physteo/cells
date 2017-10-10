#include "CellColony.h"

size_t CellColony::totalNumberOfParts() const
{
	size_t tot = 0;
	for(size_t i = 0; i < size(); i++)
		tot += m_cells.at(i).getNumOfParts();
	return tot;
}

void CellColony::addTwoPartsCell(double x1, double y1, double vx1, double vy1,
	double x2, double y2, double vx2, double vy2)
{
	size_t cell = m_cells.size();
	Part p1{ Vector{ x1, y1 }, Vector{ vx1, vy1 }, 0, cell };
	Part p2{ Vector{ x2, y2 }, Vector{ vx2, vy2 }, 1, cell };

	std::vector<Part> parts = { p1, p2 };
	Cell newCell{ parts };
	this->m_cells.push_back(newCell);
}


void CellColony::populate(double numPerLineX, double numPerLineY, double boxLengthX, double boxLengthY, double sigMin)
{
	double lx = boxLengthX / numPerLineX;
	double ly = boxLengthY / numPerLineY;
	//double sigBB = inputJson["SYSTEM"]["Type"]["sigBB"];

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
	//trajectory.push_back(colony);
}



bool CellColony::save(Hdf5* file, const char* name) const
{
	// Hdf5 was just created outside
	// create a group for cells
	file->createNewGroup(name);
	H5::Group groupCells = file->openGroup(name);

	// TODO: URGENT remove simon's unit
	//double unit = 1.0;

	// from here new
	const H5::CompType& partType = Hdf5types::getPartTypewCell();
	int LENGTH = this->totalNumberOfParts();
	int RANK = 1;
	hsize_t dim[] = { LENGTH };   /* Dataspace dimensions */
	H5::DataSpace space(RANK, dim);
	H5::DataSet* dataset = new H5::DataSet(groupCells.createDataSet("c", partType, space));
	std::vector<LightPartwCell> parts;
	parts.resize(LENGTH);
	size_t partCounter = 0;
	for (int i = 0; i < m_cells.size(); i++) {
		for (size_t j = 0; j < m_cells.at(i).getNumOfParts(); j++) {
			const Part* part = &m_cells.at(i).getPart(j);
			// TODO URGENT: remove simon's units !!
			//parts.at(partCounter) = LightPartwCell{ part->position.x * unit, part->position.y * unit, part->velocity.x * unit, part->velocity.y * unit, part->cell};
			parts.at(partCounter) = LightPartwCell{ part->position.x, part->position.y, part->velocity.x, part->velocity.y, part->cell };
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
	m_cells.resize(0);
	H5::Group groupCells = file->openGroup(name);
	H5::DataSet dataset = groupCells.openDataSet("c");
	H5::DataSpace dataspace = dataset.getSpace();
	hsize_t LENGTH = dataspace.getSimpleExtentNpoints();
	std::vector<LightPartwCell> lightParts;
	lightParts.resize(LENGTH);
	dataset.read(&lightParts.at(0), partType);

	size_t currentCell = 0;
	size_t type = 0;
	std::vector<Part> partsInCell;
	for (int i = 0; i < LENGTH; i++) {
		if (!(lightParts.at(i).cell == currentCell))
		{
			currentCell++;
			type = 0;

			m_cells.push_back(partsInCell);
			partsInCell.resize(0);
		}
		Part part{ Vector{lightParts.at(i).x, lightParts.at(i).y}, Vector{ lightParts.at(i).vx , lightParts.at(i).vy }, type, lightParts.at(i).cell };
		partsInCell.push_back(part);
		
		type++;
	}
	m_cells.push_back(partsInCell);

	return true;
}




void CellColony::push_back(const Cell& cc)
{
	m_cells.push_back(cc);
}

Cell& CellColony::at(size_t i)
{
	return m_cells.at(i);
}

const Cell& CellColony::at(size_t i) const
{
	return m_cells.at(i);
}

void CellColony::resize(size_t i)
{
	m_cells.resize(i);
}

void CellColony::reserve(size_t i)
{
	m_cells.reserve(i);
}