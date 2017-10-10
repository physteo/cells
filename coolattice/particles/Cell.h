#pragma once


#include "Part.h"
#include "vector"


class Cell
{
	std::vector<Part> m_parts;
	
public:
	Cell();
	Cell(size_t numberOfParts);
	Cell(const std::vector<Part>& partsIn);

	inline size_t getNumOfParts() const { return m_parts.size(); }

	inline const Part& getPart(size_t n) const  { return m_parts.at(n); }
	inline		 Part& getPart(size_t n)		{ return m_parts.at(n); }

	inline const std::vector<Part>& getParts() const { return m_parts; }
	inline		 std::vector<Part>& getParts() { return m_parts; }


};