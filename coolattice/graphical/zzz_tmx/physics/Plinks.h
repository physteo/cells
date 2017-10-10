#pragma once


#include "Particle.h"
#include "Pcontacts.h"

namespace tmx { namespace physics {

	// generates a contact whenever two particles violate their link
	
	class ParticleLink : public ParticleContactGenerator
	{
	public:
		Particle* particle[2];
	protected:
		ParticleLink(Particle* p1, Particle* p2)
		{
			particle[0] = p1;
			particle[1] = p2;
		}

		real currentLength() const
		{
			glm::vec3 distance = particle[0]->getPosition() - particle[1]->getPosition();
			return (real) glm::length(distance);
		}

	public:
		// fills the given contact structure with the contact needed
		// to keep the link from violating its constraint. The contact 
		// pointer give in input should point to the first available contact
		// in a contact array, where "limit" is the maximum number of contacts in the array that can be written to.
		// The method returns the number of contacts that
		// have been written. This format is common to contact-generating 
		// functions, but this class can only generate a single contact, so the 
		// pointer can be a pointer to a single element; The limit parameter is assumed to 
		// be at least one (zero is not valid), and the return value is either 0 (contact was needed)
		// or one, if a contact was needed.
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;

		// the generator is not active anymore when one of the two particles are selected
		virtual bool isGeneratorActive() const
		{
			return !((particle[0]->isSelected()) || (particle[1]->isSelected()));
		}
	};


	class ParticleCable : public ParticleLink
	{
	public:
		// the contact is generated once a maximum distance between particles is reached
		real m_maxLength;
		// bounciness of the cable
		real m_restitution;
	public:
		ParticleCable(Particle* p1, Particle* p2, real maxLength, real restitution)
			: ParticleLink(p1, p2), m_maxLength(maxLength), m_restitution(restitution) {}

		virtual unsigned addContact(ParticleContact* outContact, unsigned limit) const
		{
			unsigned addedContacts = 0;

			real penetration = currentLength() - m_maxLength;
			if (penetration < 0.0)
			{
				return 0;
			}
			
			// fill contact give in input
			outContact->restitution = m_restitution;
			glm::vec3 normal = -(particle[0]->getPosition() - particle[1]->getPosition());
			outContact->contactNormal = normal / glm::length(normal);
			outContact->particle[0] = particle[0];
			outContact->particle[1] = particle[1];
			outContact->penetration = penetration;
			outContact->solved = false;

			addedContacts++;				
			
			




			if (addedContacts > limit)
			{
				// TODO: throw some appropriate exception
				assert(false);
			}
				
			return addedContacts;
		}
	};


	class ParticleRod : public ParticleLink
	{
	private:
		real m_length;
	public:
		ParticleRod(Particle* p0, Particle* p1, real length) : ParticleLink(p0, p1), m_length(length) {}

		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const {
			unsigned addedContacts = 0;

			real penetration = currentLength() - m_length;
			glm::vec3 normal = -(particle[0]->getPosition() - particle[1]->getPosition());
			
			if (penetration == 0.0)
			{
				return 0;
			}
			else if (penetration < 0.0)
			{
				contact->contactNormal = -normal / glm::length(normal);
				contact->penetration = -penetration;
				addedContacts++;	
			}
			else if (penetration > 0.0)
			{
				contact->contactNormal = normal / glm::length(normal);
				contact->penetration = penetration;
				addedContacts++;
			}
			
			contact->restitution = 0.0;
			contact->particle[0] = particle[0];
			contact->particle[1] = particle[1];
			contact->solved = false;






			if (addedContacts > limit)
			{
				// TODO: throw some appropriate exception
				assert(false);
			}

			return addedContacts;


		}
	};


} }