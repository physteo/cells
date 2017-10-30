#include "OneBodyPropulsion.h"


OneBodyPropulsion::OneBodyPropulsion(Vector propulsion) : m_propulsion(propulsion)
{

}


void OneBodyPropulsion::updateForce(Part* part, const Box* box, Vector& forceVector) const
{
	forceVector = m_propulsion;
	part->velocity += forceVector;
}
