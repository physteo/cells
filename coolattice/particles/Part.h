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

	size_t type;
	size_t cell;
	size_t currentStage;
	size_t currentStageTime;
	double currentSigma;


	BoxCell* myBoxCell;

	Part* next;
	Part* prev;


public:
	Part() :  position(), velocity(), type(), cell(), currentStage(0), currentStageTime(0), currentSigma(1.0), myBoxCell(nullptr), next(nullptr), prev(nullptr)
	{
		currentStage = 0;
		currentStageTime = 0;
	}

	Part(const Vector& positionIn, const Vector& velocityIn, size_t typeIn, size_t cellIn, size_t stageIn, size_t currentStageTimeIn, double currentSigmaIn)
		: position{ positionIn }, velocity{ velocityIn }, type(typeIn), cell(cellIn),
		currentStage(stageIn), currentStageTime(currentStageTimeIn), currentSigma(currentSigmaIn),
		myBoxCell(nullptr), next(nullptr), prev(nullptr)
	{
		//BUG//currentStage = 0;
		//BUG//currentStageTime = 0;
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



struct LightPart {
	double x;
	double y;
	double vx;
	double vy;
};

struct LightPartwCell {
	double x;
	double y;
	double vx;
	double vy;
	size_t cell;
	size_t type;
	size_t currentStage;
	size_t currentStageTime;
	double currentSigma;

	LightPartwCell() : x(0.0), y(0.0), vx(0.0), vy(0.0), cell(0), type(0), currentStage(0),
		currentStageTime(0), currentSigma(1.0)
	{}

	LightPartwCell(double xIn, double yIn, double vxIn, double vyIn, size_t cellIn,
		size_t typeIn, size_t currentStageIn, size_t currentStageTimeIn, double currentSigmaIn)
		: x(xIn), y(yIn), vx(vxIn), vy(vyIn), cell(cellIn), type(typeIn), currentStage(currentStageIn),
		currentStageTime(currentStageTimeIn), currentSigma(currentSigmaIn)
	{}
};