#pragma once

#include "Pcontacts.h"

namespace tmx { namespace physics {


	class ParticleSphere : public ParticleContactGenerator
	{
	private:
		Particle* m_particle[2];
		real m_diameter[2];
		real m_restitution;
	public:
		ParticleSphere(Particle* p0, Particle* p1, real diameter0, real diameter1, real restitution)
		{
			m_particle[0] = p0;
			m_particle[1] = p1;

			m_diameter[0] = diameter0;
			m_diameter[1] = diameter1;

			m_restitution = restitution;
		}

		virtual bool isGeneratorActive() const
		{
			return !((m_particle[0]->isSelected()) || (m_particle[1]->isSelected()));
		}


		virtual unsigned addContact(ParticleContact* outContact, unsigned limit) const
		{
			unsigned addedContacts = 0;

			real sigma = (m_diameter[0] + m_diameter[1])/2.0;
			//
			glm::vec3 normal = m_particle[1]->getPosition() - m_particle[0]->getPosition();
			real distance = glm::length(normal);
			normal = -normal / distance;
			
			real penetration = sigma - distance;

			if (penetration < 0.0)
			{
				return 0;
			}

			// fill contact give in input
			outContact->restitution = m_restitution;
			outContact->contactNormal = normal;
			outContact->particle[0] = m_particle[0];
			outContact->particle[1] = m_particle[1];
			outContact->penetration = penetration;
			outContact->solved = false;

			addedContacts++;

			if (addedContacts > limit)
			{
				// TODO: throw some appropriate exception
				assert(false);
			}

			return 1;
		}

	};

} }