#pragma once


#include "Part.h"
#include "vector"


class Cell
{
public:
	Cell();
	Cell(size_t numberOfParts);
	Cell(const std::vector<Part>& partsIn);


	//Cell(const Cell& other) // copy constructor
	//{
	//	this->m_parts = other.m_parts;
	//	setCell();
	//}
	//
	//Cell(Cell&& other) : m_parts(other.m_parts) // move constructor
	//{
	//	setCell();
	//}
	//
	//Cell& operator=(const Cell& other) // copy assignment
	//{
	//	this->m_parts = other.m_parts;
	//	setCell();
	//	return *this;
	//}
	//Cell& operator=(Cell&& other) // move assignment
	//{
	//	if (this != &other) // prevent self-move
	//	{
	//		this->m_parts = std::move(other.m_parts);
	//		setCell();
	//	}
	//	return *this;
	//}


	void init(const std::vector<Part>& partsIn);

	inline size_t getNumOfParts() const { return m_parts.size(); }

	inline const Part& getPart(size_t n) const  { return m_parts.at(n); }
	inline		 Part& getPart(size_t n)		{ return m_parts.at(n); }

	inline const std::vector<Part>& getParts() const { return m_parts; }
	//inline		 std::vector<Part>& getParts() { return m_parts; }


private:
	std::vector<Part> m_parts;

//private:
//	void setCell();

};