
#include <assert.h>
#include "Particle.h"

namespace tmx { namespace physics {

	void Particle::init(const glm::vec3& position, const glm::vec3& velocity,
		const glm::vec3& acceleration, real inverseMass, real damping, const glm::vec3& forceAccum)
	{
		m_position = position;
		m_velocity = velocity;
		m_acceleration = acceleration;
		m_inverseMass = inverseMass;
		m_damping = damping;
		m_forceAccum = forceAccum;
	}

	void Particle::integrateEuler_Deprecated(real duration)
	{
		assert(duration > 0.0);
		
		// compute new position
		//glm::vec3 newPosition = ((real)2.0) * m_position - m_oldPosition + m_inverseMass * m_forceAccum * duration * duration;
		//m_velocity = (newPosition - m_oldPosition) / (((real)2.0) * duration);
		//m_oldPosition = m_position;
		//m_position = newPosition;
		// update position (first order in dt)
		m_position += m_velocity * duration;
		
		// calculate acceleration
		glm::vec3 resultingAcceleration = m_acceleration;
		resultingAcceleration += m_inverseMass * m_forceAccum;
		
		// update velocity (first order in dt)
		m_velocity += resultingAcceleration * duration;
		
		// impose drag
		m_velocity *= real_pow(m_damping, duration);



		clearAccumulator();
	}

	void Particle::integrate(real duration)
	{
		assert(duration > 0.0);

		m_acceleration = m_inverseMass * m_forceAccum;
		m_position = m_position + m_velocity * duration + ((real) 0.5) * m_acceleration * duration * duration;
		//m_position = m_position + m_velocity * duration + ((real) 0.5) * m_inverseMass * m_oldForceAccum * duration * duration;
		// euler
		////m_position = m_position + m_velocity * duration + ((real) 0.5) * m_inverseMass * m_oldForceAccum * duration * duration;
		////m_velocity = m_velocity + ((real) 0.5) * m_inverseMass * (m_forceAccum + m_oldForceAccum) * duration;
		m_oldForceAccum = m_forceAccum;
		clearAccumulator();
	}

	void Particle::integrateVelocity(real duration)
	{
		assert(duration > 0.0);

		m_acceleration = m_inverseMass * m_forceAccum;
		m_velocity = m_velocity + ((real) 0.5) * m_inverseMass * (m_forceAccum + m_oldForceAccum) * duration;

		m_oldForceAccum = m_forceAccum;

		//clearAccumulator();
	}



	void Particle::clearAccumulator()
	{
		m_forceAccum.x = (real) 0;
		m_forceAccum.y = (real) 0;
		m_forceAccum.z = (real) 0;
	}

	void Particle::addForce(const glm::vec3& force)
	{
		m_forceAccum += force;
	}



	void Particle::setPosition(const glm::vec3& position)
	{
		m_position = position;
	}
	void Particle::setVelocity(const glm::vec3& velocity)
	{
		m_velocity = velocity;
	}
	void Particle::setAcceleration(const glm::vec3& acceleration)
	{
		m_acceleration = acceleration;
	}
	void Particle::setInverseMass(real inverseMass)
	{
		m_inverseMass = inverseMass;
	}
	void Particle::setDamping(real damping)
	{
		m_damping = damping;
	}
	void Particle::setForceAccum(const glm::vec3& forceAccum)
	{
		m_forceAccum = forceAccum;
	}




} }