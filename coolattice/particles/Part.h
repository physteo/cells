#pragma once

#include <memory>
#include "../math/Vector.h"
//#include "../box/BoxCell.h"
#include <vector>

class BoxCell;

class Part
{
public:
	Vector position;
	Vector velocity;
	std::vector<double> data;

	size_t type;
	size_t cell;
	unsigned short stage;
	unsigned short currentStage;
	size_t currentStageTime;

	BoxCell* myBoxCell;

	Part* next;
	Part* prev;


public:
	Part() :  position(), velocity(), type(), cell(), stage(0), myBoxCell(nullptr), next(nullptr), prev(nullptr)
	{
		data.resize(0);
		currentStage = 0;
		currentStageTime = 0;
	}

	//Part(const Vector& positionIn, const Vector& velocityIn, size_t typeIn, size_t cellIn)
	//	: position{ positionIn }, velocity{ velocityIn }, type(typeIn), cell(cellIn), stage(0), myBoxCell(nullptr), next(nullptr), prev(nullptr)
	//{
	//	data.resize(0);
	//}

	Part(const Vector& positionIn, const Vector& velocityIn, size_t typeIn, size_t cellIn, size_t stageIn)
		: position{ positionIn }, velocity{ velocityIn }, type(typeIn), cell(cellIn), stage(stageIn), myBoxCell(nullptr), next(nullptr), prev(nullptr)
	{
		data.resize(0);
		currentStage = 0;
		currentStageTime = 0;
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

	inline size_t getCell() const { return this->cell; }
};