#include "Pworld.h"

namespace tmx { namespace physics {

	ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations) : resolver(iterations), maxContacts(maxContacts)
	{
		contacts.resize(maxContacts);
		calculateIterations = (iterations == 0);
	}

	// initialize the world for a simulation frame:
	// this clears the force accumulators for the particles in the world.
	// After calling this, we will apply the forces acting on the particles for this frame.
	void ParticleWorld::startFrame()
	{
		for (ParticlePointerVector::iterator p = particles.begin();
			p != particles.end(); p++)
		{
			// remove all forces from the accumulator
			(*p)->clearAccumulator();
		}

	}
	ParticleWorld::~ParticleWorld()
	{
	}

	// calls each of the registered contact generators to report
	// their contacts. Return the number of generated contacts
	unsigned ParticleWorld::generateContacts()
	{
		unsigned limit = maxContacts;
		//ParticleContact* nextContact = contacts;
		size_t contact = 0;

		// loop through all the contact generators
		for (ContactGeneratorPointerVector::iterator g = contactGenerators.begin();
			g != contactGenerators.end(); g++)
		{
			// now the contact generator tries to add a contact to the array "contacts"
			// add contact (used = 1) or dont add contact (used = 0)
			unsigned used = (*g)->addContact(&contacts.at(contact), limit);
			// if a contact was added, then the available slots decrease by one
			limit -= used;
			// if a contact was added, go to the next element element of the contacts array
			contact += used;

			// we have run out of contacts to fill. This means we are missing contacts (?)
			if (limit <= 0) { break; }
		}
		// return the number of contacts used
		return maxContacts - limit;
	}

	// integrate all the particles in this world forward in time 
	// by the given duration
	void ParticleWorld::integratePositions(real duration)
	{

		for (ParticlePointerVector::iterator p = particles.begin();
			p != particles.end(); p++)
		{
			(*p)->integrate(duration);
		}
	}

	void ParticleWorld::integrateVelocities(real duration)
	{
		for (ParticlePointerVector::iterator p = particles.begin();
			p != particles.end(); p++)
		{
			(*p)->integrateVelocity(duration);
		}
	}

	// process all the physics for the particle world
	void ParticleWorld::runPhysics(real duration)
	{


		// velocity verlet scheme: first update position
		integratePositions(duration);
		// then, update forces
		registry.updateForces(duration);
		// last, update velocities
		integrateVelocities(duration);
		
		// generate contacts
		unsigned usedContacts = generateContacts();
		// and process them
		if (calculateIterations) { resolver.setIterations(usedContacts * 2); }
		resolver.resolveContacts(contacts, usedContacts, duration);

	}


	bool is_particle_selected(const Particle* p)
	{
		return p->isSelected();
	}

	bool is_contactgenerator_not_active(const ParticleContactGenerator* cg)
	{
		return !(cg->isGeneratorActive());
	}

	void ParticleWorld::deleteSelected()
	{
		registry.removeAllSelected();
		particles.erase(std::remove_if(particles.begin(), particles.end(), is_particle_selected),
			particles.end());
		contactGenerators.erase(std::remove_if(contactGenerators.begin(), contactGenerators.end(), is_contactgenerator_not_active),
			contactGenerators.end());
	}

} }