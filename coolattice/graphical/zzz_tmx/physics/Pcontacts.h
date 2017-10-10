#pragma once 

#include "Particle.h"

namespace tmx { namespace physics {




	class ParticleContact
	{
	public:
		// the second element can be NULL, in case of contact with
		// the scenery
		Particle* particle[2];
	
		// normal restitution coefficient
		real restitution;
	
		// direction of the normal in world coords
		glm::vec3 contactNormal;

		real penetration;
	
		// corrective movement (filled only after resolution)
		glm::vec3 particleMovement[2];

		// tells if the contact was solved or not
		bool solved;
		
	public:
		ParticleContact()
		{
			particle[0] = nullptr;
			particle[1] = nullptr;
			restitution = (real) 0.0;
			penetration = (real) 0.0;
			solved = false;
		}
		ParticleContact(Particle* in_particle[2], real in_restitution, glm::vec3 in_contactNormal, real in_penetration)
		{
			particle[0] = in_particle[0];
			particle[1] = in_particle[1];
			restitution = in_restitution;
			contactNormal = in_contactNormal;
			penetration = in_penetration;
			solved = false;
		}
		
		
	public:
		void resolve(real duration)
		{
			resolveVelocity(duration);
			resolveInterpenetration(duration);
			solved = true;
		}
	
		real calculateSeparatingVelocity() const
		{
			glm::vec3 relativeVelocity = particle[0]->getVelocity();
			if (particle[1]) relativeVelocity -= particle[1]->getVelocity();
			return glm::dot(relativeVelocity,contactNormal);
		}
	
	private:
		void resolveInterpenetration(real duration)
		{
			if (penetration <= 0) { return; }
			real totalInverseMass = particle[0]->getInverseMass();
			if (particle[1]) { totalInverseMass += particle[1]->getInverseMass(); }
			
			if (totalInverseMass <= 0) { return; }

			glm::vec3 moveOverMass = contactNormal * penetration / totalInverseMass;
			particleMovement[0] = moveOverMass * particle[0]->getInverseMass();
			particle[0]->setPosition(particle[0]->getPosition() + particleMovement[0]);
			if (particle[1])
			{
				particleMovement[1] = -moveOverMass * particle[1]->getInverseMass();
				particle[1]->setPosition(particle[1]->getPosition() + particleMovement[1]);
			}


		}

		// impulse calculation here
		void resolveVelocity(real duration)
		{
			// find velocity in the direction of the contact
			real separatingVelocity = calculateSeparatingVelocity();
	
			if (separatingVelocity > 0)
			{
				// the contact is either separating or stationary
				// no need for applying an impulse
				return;
			}
	
			// calculate new separating velocity
			real newSepVelocity = -separatingVelocity * restitution;
	

			// check the velocity build-up due to acceleration only
			// in the direction of the normal
			glm::vec3 accCausedVelocity = particle[0]->getAcceleration();
			if (particle[1]) { accCausedVelocity -= particle[1]->getAcceleration(); }
			real accCausedSepVelocity = glm::dot(accCausedVelocity,contactNormal) * duration;
			// If we have a closing velocity due to acceleration only
			// remove it from the separating velocity
			if (accCausedSepVelocity < 0)
			{
				newSepVelocity += restitution * accCausedSepVelocity;
			
				// Make sure we have not removed more than was there to remove
				if (newSepVelocity < 0) { newSepVelocity = 0; }
			}


			real deltaVelocity = newSepVelocity - separatingVelocity;
	
			real totalInverseMass = particle[0]->getInverseMass();
			if (particle[1]) totalInverseMass += particle[1]->getInverseMass();
			if (totalInverseMass <= 0) { return; }
	
	
			glm::vec3 impulseOverMass = contactNormal * deltaVelocity / totalInverseMass;
	
			particle[0]->setVelocity(particle[0]->getVelocity() +
				impulseOverMass * particle[0]->getInverseMass());
			if (particle[1])
			{
				particle[1]->setVelocity(particle[1]->getVelocity() -
					impulseOverMass * particle[1]->getInverseMass());
			}
		}
	
	
	
	};

	// Contact resolution routine for particle contacts. 
	// One resolver instance can be shared for the whole simulation
	class ParticleContactResolver
	{
	protected:
		// maximum number of iterations
		unsigned m_iterations;

		// actual number of iterations
		unsigned m_iterationsUsed;

	public:
		ParticleContactResolver(unsigned iterations) : m_iterations(iterations) {}

		void setIterations(unsigned iterations)
		{
			m_iterations = iterations;
		}

		// Resolves a set of particle contacts both for
		// penetration and velocity
		void resolveContacts(std::vector<ParticleContact>& contactArray, unsigned numContacts,
			real duration)
		{
			m_iterationsUsed = 0;
			while (m_iterationsUsed < m_iterations)
			{
				// most negative separating velocity is found
				real max = 0;
				unsigned maxIndex = numContacts;

				for (unsigned i = 0; i < numContacts; i++)
				{
					real sepVel = contactArray[i].calculateSeparatingVelocity();
					if (sepVel < max && sepVel < 0 || contactArray[i].penetration > 0)
					{
						max = sepVel;
						maxIndex = i;
					}
				}

				// check if there is any contact to resolve
				if (maxIndex == numContacts) {
					break;
				}
				
				// solve this contact, both for velocity and interpenetration
				contactArray[maxIndex].resolve(duration);
				contactArray[maxIndex].solved = true;

				glm::vec3 correctionMove[2] = { contactArray[maxIndex].particleMovement[0],  contactArray[maxIndex].particleMovement[1] };


				// update all contacts which have either particle0 or particle1 of the worst contact
				for (unsigned i = 0; i < numContacts; i++)
				{

					if (contactArray[i].particle[0] == contactArray[maxIndex].particle[0])
					{
						// minus sign because the normal vector is pointing from particle1 to particle0 !
						contactArray[i].penetration -= glm::dot(correctionMove[0], contactArray[i].contactNormal);
					}
					else if (contactArray[i].particle[0] == contactArray[maxIndex].particle[1])
					{
						contactArray[i].penetration -= glm::dot(correctionMove[1], contactArray[i].contactNormal);
					}
						
					if (contactArray[i].particle[1])
					{
						if (contactArray[i].particle[1] == contactArray[maxIndex].particle[0])
						{
							contactArray[i].penetration += glm::dot(correctionMove[0], contactArray[i].contactNormal);
						}
						else if (contactArray[i].particle[1] == contactArray[maxIndex].particle[1])
						{
							contactArray[i].penetration += glm::dot(correctionMove[1], contactArray[i].contactNormal);
						}

					}


				}


				m_iterationsUsed++;
				
			} // end while



		}




	};


	// polymorphic interface for contact generators
	// applying to particles
	class ParticleContactGenerator
	{
	public:
		// fills the given contact structure with the generated contact. The contact 
		// pointer given in input should point to the first available contact
		// in a contact array, where "limit" is the maximum number of contacts in the array that can be written to.
		// The method returns the number of contacts that
		// have been written.
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;

		// provides a way to check whether the contact generator is still active
		// useful when we delete some objects from the game and want to delete the 
		// contact generators associated with them
		virtual bool isGeneratorActive() const = 0;
	};

	class ParticleGroundContact : public ParticleContactGenerator
	{
	private:
		Particle* m_particle;
		real m_diameter;
		real m_restitution;
	public:
		ParticleGroundContact(Particle* particle, real diameter, real restitution) : m_particle(particle), m_diameter(diameter), m_restitution(restitution) {}

		virtual bool isGeneratorActive() const
		{
			return !(m_particle->isSelected());
		}

		virtual unsigned addContact(ParticleContact* outContact, unsigned limit) const
		{
			unsigned addedContacts = 0;

			
			
			glm::vec3 normal{ 0.0,1.0,0.0 };
			
			real penetration = m_diameter/2.0 - m_particle->getPosition().y;

			if (penetration < 0.0)
			{
				return 0;
			}

			// fill contact give in input
			outContact->restitution = m_restitution;
			outContact->contactNormal = normal;
			outContact->particle[0] = m_particle;
			outContact->particle[1] = nullptr;
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

	// just to test
	//void solveContact(Particle* p1, Particle* p2, real duration)
	//{
	//	real diameter = 2.0;
	//
	//	glm::vec3 normal = p2->getPosition() - p1->getPosition();
	//	real distance = glm::length(normal);
	//	normal = -normal / distance;
	//	Particle* particles[2] = { p1, p2 };
	//
	//	real penetration = diameter - distance;
	//
	//	if (distance < diameter) {
	//		ParticleContact pc{ particles, 1.0, normal, penetration };
	//		pc.resolve(duration);
	//	}
	//}
	

	// new stuff
	//bool is_particle_in_contact_generator_selected()
	//{
	//
	//}
	//
	//class ContactGeneratorPointerVector2
	//{
	//public:
	//	std::vector<ParticleContactGenerator*> vector;
	//	void removeAllSelected()
	//	{
	//
	//	}
	//};


} }

