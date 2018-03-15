#include "Box.h"

Box::Box() : m_lx(0.0), m_ly(0.0), m_periodicBoundaries(true)
{
}


Box::Box(double lx, double ly, bool periodicBoundaries, int subBoxesX, int subBoxesY) : m_lx(lx), m_ly(ly), m_periodicBoundaries(periodicBoundaries)
{	
	m_numberOfBoxCellsX = subBoxesX;
	m_numberOfBoxCellsY = subBoxesY;


	m_subBoxesLengthX = lx / subBoxesX;
	m_subBoxesLengthY = ly / subBoxesY;

	std::cout << "Box lx:" << m_lx  << std::endl;
	std::cout << "Box ly:" << m_ly << std::endl;

	createCubic();

}


void Box::createCubic()
{
	m_subBoxes.resize(m_numberOfBoxCellsX * m_numberOfBoxCellsY);
	// for now, only cubic
	for (int x = 0; x < m_numberOfBoxCellsX; x++)
	{
		for (int y = 0; y < m_numberOfBoxCellsY; y++)
		{
			// initialize the cell
			BoxCell& subBox = m_subBoxes.at(getBoxCellIndex(x, y));

			subBox.head.clear();

			// set boundaries of the cell
			subBox.downLeftCorner =   { ((double)x) * m_subBoxesLengthX, ((double)y) * m_subBoxesLengthY };
			subBox.upperRightCorner = { (((double)x) + 1) * m_subBoxesLengthX, (((double)y) + 1) * m_subBoxesLengthY };

			// set neighbours
			// TODO: case of no periodic boundaries
			subBox.neighbour[0] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x - 1, m_numberOfBoxCellsX), wrapIndex(y - 1, m_numberOfBoxCellsY))));
			subBox.neighbour[1] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x - 1, m_numberOfBoxCellsX), wrapIndex(y + 0, m_numberOfBoxCellsY))));
			subBox.neighbour[2] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x - 1, m_numberOfBoxCellsX), wrapIndex(y + 1, m_numberOfBoxCellsY))));
			subBox.neighbour[3] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x + 0, m_numberOfBoxCellsX), wrapIndex(y + 1, m_numberOfBoxCellsY))));
			subBox.neighbour[4] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x + 1, m_numberOfBoxCellsX), wrapIndex(y + 1, m_numberOfBoxCellsY))));
			subBox.neighbour[5] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x + 1, m_numberOfBoxCellsX), wrapIndex(y + 0, m_numberOfBoxCellsY))));
			subBox.neighbour[6] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x + 1, m_numberOfBoxCellsX), wrapIndex(y - 1, m_numberOfBoxCellsY))));
			subBox.neighbour[7] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x + 0, m_numberOfBoxCellsX), wrapIndex(y - 1, m_numberOfBoxCellsY))));
			subBox.neighbour[8] = &(m_subBoxes.at(getBoxCellIndex(wrapIndex(x + 0, m_numberOfBoxCellsX), wrapIndex(y + 0, m_numberOfBoxCellsY)))); // itself
		}
	}
}


void Box::putPartInSubBox(Part* part, int n)
{
	int tempX = static_cast<int> (part->position.x / m_subBoxesLengthX);
	int tempY = static_cast<int> (part->position.y / m_subBoxesLengthY);

#ifdef DEBUG
	std::cout << "particle " << part->cell << "/" << part->type << " has temp box indices " << tempX << "," << tempY << std::endl;
#endif

	int x = wrapXIndex(tempX);
	int y = wrapYIndex(tempY);


	int subBoxIndex = getBoxCellIndex(x, y);

#ifdef DEBUG
	std::cout << "particle " << particl->cell << "/" << particle->type << " is at " << subBoxIndex << std::endl;
#endif

	BoxCell* subBox = &getBoxCells().at(subBoxIndex);
	part->myBoxCell = subBox;

//#ifdef OMP	
//#pragma omp critical
//#endif
	{
		part->remove(); 
		subBox->head.insert(part);
	}

}


void Box::clearSubBoxes()
{
	for (size_t i = 0; i < getBoxCells().size(); i++)
	{
		getBoxCells().at(i).head.clear();
	}
}

void Box::remap(Vector &p)
{

	if (p.x >= m_lx) {
		p.x -= m_lx;
	}
	else if (p.x < 0.0) {
		p.x += m_lx;
	}
	if (p.y >= m_ly) {
		p.y -= m_ly;
	}
	else if (p.y < 0.0) {
		p.y += m_ly;
	}

	// check if the particle was too far outside (which could happen after an overlap
	if (p.x > m_lx || p.x < 0.0 || p.y > m_ly || p.y < 0.0)
	{
		std::cerr << "Particle jumped far out of the box." << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}

	// old remapping
	/*
	while (p.x >= m_lx)
		p.x -= m_lx;
	while (p.x < 0.0)
		p.x += m_lx;
	while (p.y >= m_ly)
		p.y -= m_ly;
	while (p.y < 0.0)
		p.y += m_ly;
	*/
}




// returns the square of the distance in the box, and the distance vector in the last argument
double Box::computeDistanceSquaredPBC(const Vector& v1, const Vector& v2, Vector& output) const
{
	output = v1 - v2;
	double& deltax = output.x;
	double& deltay = output.y;

	// periodic boundary conditions
	
	
	while (deltax > 0.5 * m_lx)
	{
		deltax -= m_lx;
	}
	while (deltax <= -0.5 * m_lx)
	{
		deltax += m_lx;
	}
	while (deltay > 0.5 * m_ly)
	{
		deltay -= m_ly;
	}
	while (deltay <= -0.5 * m_ly)
	{
		deltay += m_ly;
	}
	


	return Vector::dotProduct(output, output);

}

// returns the square of the distance in the box, and the distance vector in the last argument
void Box::computeDistanceVectorPBC(const Vector& v1, const Vector& v2, Vector& output) const
{
	output = v1 - v2;
	double& deltax = output.x;
	double& deltay = output.y;

	// periodic boundary conditions

	
	while (deltax > 0.5 * m_lx)
	{
	deltax -= m_lx;
	}
	while (deltax <= -0.5 * m_lx)
	{
	deltax += m_lx;
	}
	while (deltay > 0.5 * m_ly)
	{
	deltay -= m_ly;
	}
	while (deltay <= -0.5 * m_ly)
	{
	deltay += m_ly;
	}


}



bool Box::save(Hdf5* file, const char* groupName) const
{
	try {
		H5::Group group = file->createGroupNoThrow(groupName);

		file->writeAttributeDouble(groupName, "lx", m_lx);
		file->writeAttributeDouble(groupName, "ly", m_ly);
		file->writeAttributeDouble(groupName, "lcx", m_subBoxesLengthX);
		file->writeAttributeDouble(groupName, "lcy", m_subBoxesLengthY);
		file->writeAttributeBool(groupName, "pbc", m_periodicBoundaries);
		file->writeAttributeDouble(groupName, "subBoxesX", m_numberOfBoxCellsX);
		file->writeAttributeDouble(groupName, "subBoxesY", m_numberOfBoxCellsY);

		return true;
	}
	catch (H5::Exception &err)
	{
		// TODO: write to a logger
		// err.printError(loggerFilename...);
		err.printError();
		return false;
	}
}


bool Box::load(Hdf5* file, const char* groupName)
{

	try {
		H5::Group group = file->openGroup(groupName);
		m_lx = file->readAttributeDouble(groupName, "lx");
		m_ly = file->readAttributeDouble(groupName, "ly");
		m_subBoxesLengthX = file->readAttributeDouble(groupName, "lcx");
		m_subBoxesLengthY = file->readAttributeDouble(groupName, "lcy");
		m_periodicBoundaries = file->readAttributeBool(groupName, "pbc");
		m_numberOfBoxCellsX = file->readAttributeInteger(groupName, "subBoxesX");
		m_numberOfBoxCellsY = file->readAttributeInteger(groupName, "subBoxesY");

		createCubic();
		return true;
	}
	catch (H5::Exception &err)
	{
		// TODO write to a logger: err.printError(loggerfilename);
		err.printError();
		return false;
	}
}
