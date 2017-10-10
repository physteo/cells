#include "Cell.h"


Cell::Cell()
{
	m_parts.resize(0);
}

Cell::Cell(size_t numberOfParts)
{
	m_parts.resize(numberOfParts);
}

Cell::Cell(const std::vector<Part>& partsIn)
{
	m_parts.reserve(partsIn.size());
	for (size_t i = 0; i < partsIn.size(); i++) {
		m_parts.push_back(partsIn.at(i));
	}

}
