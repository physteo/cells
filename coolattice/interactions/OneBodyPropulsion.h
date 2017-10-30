#pragma once

#include <cmath>
#include "Force.h"

class OneBodyPropulsion : public OneBodyForce
{
	Vector m_propulsion;


public:
	OneBodyPropulsion(Vector propulsion);

	~OneBodyPropulsion() {}

public:
	void updateForce(Part* part, const Box* box, Vector& forceVector) const  override;

};