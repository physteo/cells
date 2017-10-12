#pragma once

#include "../math/Vector.h"
#include "../particles/Part.h"

class OneBodyForce
{
public:
	virtual ~OneBodyForce() {}
	virtual void updateForce(Part* part, Vector& forceVector) const = 0;
};


class NoForce : public OneBodyForce
{
public:

public:
	NoForce() {}
	~NoForce() {}
	void updateForce(Part* part, Vector& forceVector) const override { forceVector = Vector{}; };
};

