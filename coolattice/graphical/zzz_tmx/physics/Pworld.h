#pragma once
#include "Plinks.h"
#include "Psphere.h"
#include "Pfgen.h"

namespace tmx { namespace physics {



	class ParticleWorld
	{
	public:
		typedef std::vector<Particle*> ParticlePointerVector;
		typedef std::vector<ParticleContactGenerator*> ContactGeneratorPointerVector;

		// holds pointers to the particles (which will be created outside)
		ParticlePointerVector particles;

		// holds the force generators for the particles in this world
		ParticleForceRegistry registry;

		// hold pointers to the contact generators (which will be created outside)
		ContactGeneratorPointerVector contactGenerators;

	protected:
		// holds the resolver for contacts
		ParticleContactResolver resolver;
		// holds the list of contacts
		std::vector<ParticleContact> contacts;
		// maximum number of contacts
		unsigned maxContacts;
		// decide whether to calculate iterations or not
		bool calculateIterations;

	public:
		// creates a new particle simulator that can handle up to the given
		// number of contacts per frame. you can also optionally give a number
		// of contact-resolution iterations to use. if you don't give a number of 
		// iterations, then twice the number of contacts will be used (?)
		ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
		~ParticleWorld();

		// delete copy/move assignment and constructors
		ParticleWorld(const ParticleWorld& other)		= delete;
		ParticleWorld(ParticleWorld&& other)			= delete;
		ParticleWorld& operator=(const ParticleWorld&)	= delete;
		ParticleWorld& operator=(ParticleWorld&& other) = delete;

		// initialize the world for a simulation frame:
		// this clears the force accumulators for the particles in the world.
		// After calling this, we will apply the forces acting on the particles for this frame.
		void startFrame();


		// process all the physics for the particle world
		void runPhysics(real duration);

		// delete everything that has to do with those particles which have been selected for deletion
		// It deleted entries in "particles", "registry", and "contactgenerators" members
		void deleteSelected();

	private:
		// calls each of the registered contact generators to report
		// their contacts. Return the number of generated contacts
		unsigned generateContacts();

		// integrate all the particles in this world forward in time 
		// by the given duration
		void integratePositions(real duration);
		void integrateVelocities(real duration);

	};


} }