#pragma once

#include <vector>
#include <algorithm>
#include "Particle.h"

namespace tmx { namespace physics{


	class ParticleForceGenerator
	{
	public:
		virtual void updateForce(Particle* particle, real duration) = 0;

	};



	struct ParticleForceRegistration {
		ParticleForceRegistration(Particle* in_particle, ParticleForceGenerator* in_fg)
			: particle(in_particle), fg(in_fg) {}
		bool operator==(const ParticleForceRegistration& rhs) 
			{return (this->particle == rhs.particle && this->fg == rhs.fg) ? true : false;}
		Particle* particle;
		ParticleForceGenerator* fg;
	};


	class ParticleForceRegistry
	{
	protected:

		typedef std::vector<ParticleForceRegistration> Registry;
		
		Registry registrations;
		
	public:
		void add(Particle* particle, ParticleForceGenerator* fg);
		void remove(Particle* particle, ParticleForceGenerator* fg);
		void removeAllSelected();
		void clear();
		void updateForces(real duration) const;
	};




	class ParticleGravity : public ParticleForceGenerator
	{
	private:
		glm::vec3 m_gravity;
	public:
		ParticleGravity(const glm::vec3& gravity) : m_gravity(gravity) {}
		virtual void updateForce(Particle* particle, real duration) override;
	};


	class ParticleDrag : public ParticleForceGenerator
	{
	private:
		real m_k1;
		real m_k2;
	public:
		ParticleDrag(real k1, real k2) : m_k1(k1), m_k2(k2) {}
		virtual void updateForce(Particle* particle, real duration) override;
	};


	class ParticleSpring : public ParticleForceGenerator
	{
	private:
		Particle* m_other;
		real m_springConstant;
		real m_restLenght;
	public:
		ParticleSpring(Particle* other, real springConstant, real restLength) :
			m_other(other), m_springConstant(springConstant), m_restLenght(restLength) {}

		virtual void updateForce(Particle* particle, real duration) override;
	};





} }
