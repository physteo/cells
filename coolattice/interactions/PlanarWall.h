#pragma once
#include "Force.h"

class PlanarWall : public OneBodyForce
{
	double m_sigma;
	double m_epsilon;
	double m_cut;

	int m_axis;
	double m_position;

public:
	PlanarWall();
	PlanarWall(double sigma, double epsilon, double cut, int axis, double position );

	void updateForce(Part* part, const Box* box, Vector& forceVector) const  override;

};