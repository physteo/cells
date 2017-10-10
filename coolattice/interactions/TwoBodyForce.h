#pragma once
#include "../box/Box.h"

class TwoBodyForce
{
public:
	~TwoBodyForce() {}
	virtual void updateForce(Part* part1, const Part* part2, const Box* box) const = 0;
};


class TwoBodyNoForce : public TwoBodyForce
{
public:

public:
	TwoBodyNoForce() {}
	~TwoBodyNoForce() {}
	virtual void updateForce(Part* part1, const Part* part2, const Box* box) const override {}

};