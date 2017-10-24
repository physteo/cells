#include "PlanarWall.h"

PlanarWall::PlanarWall() :
	m_sigma(0.0), m_epsilon(0.0), m_cut(0.0), m_axis(-1), m_position(0.0)
{
}


PlanarWall::PlanarWall(double sigma, double epsilon, double cut, int axis, double position) :
	m_sigma(sigma), m_epsilon(epsilon), m_cut(cut), m_axis(axis), m_position(position)
{

}

void PlanarWall::updateForce(Part* part, const Box* box, Vector& forceVector) const
{
	Vector distance{};
	Vector wallPosition{};
	double r;
	double r2;

	if (m_axis == 0)
	{
		r2 = box->computeDistanceSquaredPBC(part->position, Vector{ m_position, part->position.y }, distance);
		r = distance.x;
	}
	else
	if (m_axis == 1)
	{
		r2 = box->computeDistanceSquaredPBC(part->position, Vector{ part->position.x, m_position }, distance);
		r = distance.y;
	}
	

	if (r < m_cut) {

		double one_over_r2 = 1.0 / r2;
		double sigma2_over_r2 = m_sigma * m_sigma / r2;
		double sigma6_over_r6 = sigma2_over_r2 * sigma2_over_r2 * sigma2_over_r2;

		double force = 48.0 * m_epsilon * one_over_r2 * sigma6_over_r6 * sigma6_over_r6; 

		forceVector = distance * force;
		part->velocity += forceVector;

	}
}



void PlanarWall::setEpsilon(double newEpsilon)
{
	m_epsilon = newEpsilon;
}
