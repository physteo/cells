#pragma once

#include <memory>
#include "../math/Vector.h"
//#include "../box/BoxCell.h"

class BoxCell;

class Part
{
public:
	Vector position;
	Vector velocity;
	
	size_t type;
	size_t cell;

	BoxCell* myBoxCell;

	Part* next;
	Part* prev;


public:
	Part() :  position(), velocity(), type(), cell(), myBoxCell(nullptr), next(nullptr), prev(nullptr)
	{
	}

	Part(const Vector& positionIn, const Vector& velocityIn, size_t typeIn, size_t cellIn)
		: position{ positionIn }, velocity{ velocityIn }, type(typeIn), cell(cellIn),  myBoxCell(nullptr), next(nullptr), prev(nullptr)
	{
	}


	void insert(Part* newOne)
	{
		newOne->prev = this;
		newOne->next = this->next;
		if (this->next != nullptr)
		{
			this->next->prev = newOne;
		}
		this->next = newOne;
	}

	void remove()
	{
		if (next != nullptr) {
			next->prev = prev;
		}
		if (prev != nullptr) {
			prev->next = next;
		}
		next = prev = nullptr;
	}

	void clear()
	{
		next = prev = nullptr;
	}


};