#pragma once
#include "precision.h"
#include "../maths/maths.h"
#include "../graphics/Renderable.h"
#include "../gameObjects/ObjectWithID.h"

namespace tmx { namespace physics {
	/*
	class Particle
	{
	private:
		glm::vec3 m_position;

		graphics::Renderable renderable;
		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;

		real m_inverseMass;
		real m_damping;

		glm::vec3 m_forceAccum;

	public:
		Particle() : m_position{ 0,0,0 }, m_velocity{ 0,0,0 }, m_acceleration{ 0,0,0 }, m_inverseMass((real)0),
			m_damping((real)0), m_forceAccum{ 0,0,0 } {}

		Particle(const glm::vec3& position, const glm::vec3& velocity,
			const glm::vec3& acceleration, real inverseMass, real damping, const glm::vec3& forceAccum)
			: m_position(position), m_velocity(velocity), m_acceleration(acceleration), m_inverseMass(inverseMass),
			m_damping(damping), m_forceAccum(forceAccum) {}

		inline const glm::vec3& getPosition() const { return m_position; }
		inline const glm::vec3& getVelocity() const { return m_velocity; }
		inline const glm::vec3& getAcceleration() const { return m_acceleration; }
		inline real getInverseMass() const { return m_inverseMass; }
		inline real getDamping() const { return m_damping; }
		inline const glm::vec3& getForceAccum() const { return m_forceAccum; }

		void setPosition(const glm::vec3& position);
		void setVelocity(const glm::vec3& velocity);
		void setAcceleration(const glm::vec3& acceleration);
		void setInverseMass(real inverseMass);
		void setDamping(real damping);
		void setForceAccum(const glm::vec3& forceAccum);

		// copy, move constructors are the default ones

		void addForce(const glm::vec3& force);
		void clearAccumulator();
		void integrate(real duration);
	};
	*/

	class Particle : public gameObjects::SelectableObjectWithID
	{
		glm::vec3 m_position;
		
		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;

		real m_inverseMass;
		real m_damping;

		glm::vec3 m_forceAccum;
		glm::vec3 m_oldForceAccum;

	public:
		Particle() : SelectableObjectWithID(), m_position{ 0,0,0 }, m_velocity { 0, 0, 0 }, m_acceleration{ 0,0,0 }, m_inverseMass((real)0),
			m_damping((real)0), m_forceAccum{ 0,0,0 }, m_oldForceAccum{ 0,0,0 } {}
		
		Particle(const glm::vec3& position, const glm::vec3& velocity,
			const glm::vec3& acceleration, real inverseMass, real damping, const glm::vec3& forceAccum)
			: SelectableObjectWithID(false, 0), m_position(position), m_velocity(velocity), m_acceleration(acceleration), m_inverseMass(inverseMass),
			m_damping(damping), m_forceAccum(forceAccum), m_oldForceAccum(glm::vec3{0.0,0.0,0.0}) {}

		void init(const glm::vec3& position, const glm::vec3& velocity,
			const glm::vec3& acceleration, real inverseMass, real damping, const glm::vec3& forceAccum);

		inline const glm::vec3& getPosition() const { return m_position; }
		inline const glm::vec3* getPointerToPosition() const { return &m_position; }

		inline const glm::vec3& getVelocity() const { return m_velocity; }
		inline const glm::vec3& getAcceleration() const { return m_acceleration; }
		inline real getInverseMass() const { return m_inverseMass; }
		inline real getDamping() const { return m_damping; }
		inline const glm::vec3& getForceAccum() const { return m_forceAccum; }

		void setPosition(const glm::vec3& position);
		void setVelocity(const glm::vec3& velocity);
		void setAcceleration(const glm::vec3& acceleration);
		void setInverseMass(real inverseMass);
		void setDamping(real damping);
		void setForceAccum(const glm::vec3& forceAccum);

		// copy, move constructors are the default ones

		void addForce(const glm::vec3& force);
		void clearAccumulator();
		void integrate(real duration);
		void integrateEuler_Deprecated(real duration);

		void integrateVelocity(real duration);

	};

} }