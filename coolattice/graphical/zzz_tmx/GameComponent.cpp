#include "GameComponent.h"

namespace tmx {

	GameComponent::GameComponent(graphics::Renderable* renderable, physics::Particle* particle,
		const glm::vec3& worldPosition)
	{
		m_renderable = renderable;
		m_particle = particle;
		
		m_renderable->position = worldPosition;
		m_particle->setPosition(worldPosition);
	}

	GameComponent::~GameComponent()
	{
		delete m_renderable;
		// (TODO) fix TEMP:
		delete m_particle;
	}

	void GameComponent::updatePosition(physics::real duration)
	{
		m_particle->integrate(duration);
		sinchronize();
	}

	void GameComponent::updateVelocity(physics::real duration)
	{
		m_particle->integrateVelocity(duration);
	}


	void GameComponent::sinchronize()
	{
		m_renderable->position = m_particle->getPosition();
	}

	/*
	void GameComponent::update(const physics::ParticleForceRegistry& registry, physics::real duration)
	{
		updatePosition(duration);
		registry.updateForces(duration);
		updateVelocity(duration);
	}
	*/
}