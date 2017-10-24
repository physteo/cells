#pragma once

#include "../math/Vector.h"
#include "../particles/Part.h"
#include "../box/Box.h"

class OneBodyForce
{
public:
	virtual ~OneBodyForce() {}
	virtual void updateForce(Part* part, const Box* box, Vector& forceVector) const = 0;
};


class NoForce : public OneBodyForce
{
public:

public:
	NoForce() {}
	~NoForce() {}
	void updateForce(Part* part, const Box* box, Vector& forceVector) const override { forceVector = Vector{}; };
};

