#include "Pfgen.h"

namespace tmx { namespace physics{

	void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
	{
		registrations.emplace_back(particle, fg);
	}

	void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
	{
		ParticleForceRegistration pfr{particle, fg};
		registrations.erase(std::remove(registrations.begin(), registrations.end(), pfr), registrations.end());
	}


	bool is_particle_in_registration_selected(const ParticleForceRegistration& pfr)
	{
		return pfr.particle->isSelected();
	}

	void ParticleForceRegistry::removeAllSelected()
	{
		registrations.erase(std::remove_if(registrations.begin(), registrations.end(), is_particle_in_registration_selected), registrations.end());
	}


	void ParticleForceRegistry::clear()
	{
		registrations.clear();
	}

	void ParticleForceRegistry::updateForces(real duration) const
	{
		//Registry::iterator it = registrations.begin();
		//for (; it != registrations.end(); it++)
		//{
		//	it->fg->updateForce(it->particle, duration);
		//}
		for (size_t n = 0; n < registrations.size(); n++)
		{
			const ParticleForceRegistration& registration = registrations.at(n);
			registration.fg->updateForce(registration.particle, duration);
		}
	}


	// gravity
	void ParticleGravity::updateForce(Particle* particle, real duration)
	{
		if (particle->getInverseMass() == 0.0) return;
		particle->addForce(m_gravity * particle->getInverseMass());
	}

	// drag
	void ParticleDrag::updateForce(Particle* particle, real duration)
	{
		glm::vec3 force;
		force = particle->getVelocity();
		
		real dragCoeff = glm::length(force);
		dragCoeff = m_k1 * dragCoeff + m_k2 * dragCoeff * dragCoeff;

		if (glm::length(force) > 0.0) {
			force = glm::normalize(force);
		}
		force *= -dragCoeff;

		particle->addForce(force);
	}

	// spring
	void ParticleSpring::updateForce(Particle* particle, real duration)
	{
		if (particle->getInverseMass() == 0.0) return;
		//real fx = -m_springConstant * (particle->getPosition().x - m_other->getPosition().x);
		//glm::vec3 force;
		//force.x = fx;

		glm::vec3 force;
		force = particle->getPosition();
		force += -m_other->getPosition();
		real magnitude = glm::length(force);
		magnitude = real_abs(magnitude - m_restLenght);
		magnitude *= -m_springConstant;
		force = force / glm::length(force);
		
		force *= magnitude;
		particle->addForce(force);
	}

} }