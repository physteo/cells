#include <memory>

#include "graphics/Renderable.h"
#include "physics/Particle.h"
#include "maths/maths.h"

#include "physics/Pfgen.h"

#include "../graphics/Window.h"

namespace tmx
{


	//class GameComponentBase
	//{
	//protected:
	//
	//public:
	//	virtual 
	//	virtual void sinchronize();
	//};

	class GameComponent
	{
	protected:
		graphics::Renderable* m_renderable;
		physics::Particle* m_particle;

		//glm::vec3 m_worldPosition;

	public:
		GameComponent(graphics::Renderable* renderable, physics::Particle* particle, const glm::vec3& worldPosition);
		~GameComponent();

		const graphics::Renderable* getRenderable() const { return m_renderable; }
		const physics::Particle* getParticle() const { return m_particle; }
		// non const overriden: needed because I pass the particle pointer to the force registry
		// that update the forces inside the particle
		physics::Particle* getParticle() { return m_particle; }
		//const glm::vec3& getWorldPosition() const { return m_worldPosition; }

		void updatePosition(physics::real duration);
		void updateVelocity(physics::real duration);

		void sinchronize();

		//void update(const physics::ParticleForceRegistry& registry, physics::real duration);
	};

	

	class GameComponentPlayer : public GameComponent
	{
	private:
		const sparky::graphics::Window* const m_window;

	public:
		GameComponentPlayer(graphics::Renderable* renderable, physics::Particle* particle, const glm::vec3& worldPosition, const sparky::graphics::Window* const window)
			: GameComponent(renderable, particle, worldPosition), m_window(window) {}

		void checkInputs()
		{
			if (m_window->isKeyPressed(GLFW_KEY_I)) {
				glm::vec3 vec = m_particle->getPosition();
				vec.z += 0.1f;
				m_particle->setVelocity(vec);
			}
			else if (m_window->isKeyPressed(GLFW_KEY_K)) {
				glm::vec3 vec = m_particle->getPosition();
				vec.z -= 0.1f;
				m_particle->setVelocity(vec);
			}
			
			if (m_window->isKeyPressed(GLFW_KEY_L)) {
				glm::vec3 vec = m_particle->getPosition();
				vec.x -= 0.1f;
				m_particle->setVelocity(vec);
			}
			else if (m_window->isKeyPressed(GLFW_KEY_J)) {
				glm::vec3 vec = m_particle->getPosition();
				vec.x += 10.1f;
				m_particle->setVelocity(vec);
			}

			if (m_window->isKeyPressed(GLFW_KEY_SPACE)) {
				glm::vec3 vec = m_particle->getPosition();
				vec.y += 5.0f;
				m_particle->setVelocity(vec);
			}


		}

	};

}