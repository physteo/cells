#include "Cell.h"


Cell::Cell()
{
	m_parts.resize(0);
}

Cell::Cell(size_t numberOfParts)
{
	m_parts.resize(numberOfParts);
	//setCell();
}

Cell::Cell(const std::vector<Part>& partsIn)
{
	m_parts.reserve(partsIn.size());
	for (size_t i = 0; i < partsIn.size(); i++) {
		m_parts.push_back(partsIn.at(i));
	}
	//setCell();
}

void Cell::init(const std::vector<Part>& partsIn)
{
	this->m_parts = partsIn;
	//setCell();
}


//void Cell::setCell()
//{
//	for (size_t i = 0; i < this->getNumOfParts(); i++)
//	{
//		this->m_parts.at(i).cell = this;
//	}
//}
