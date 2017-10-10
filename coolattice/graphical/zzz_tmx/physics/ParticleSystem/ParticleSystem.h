#pragma once

#include <algorithm>

#include "../Particle.h"
#include "../../../../ObjectPool.h"
#include "./../../utils/quickRandom.h"
#include "../Pfgen.h"
#include "../../../graphics/Window.h"

#include "../../graphics/Renderable.h"

namespace tmx { namespace physics { namespace particlesystem {


	struct Source
	{
		glm::vec3 center;
		float sourceLifeTime;
		float startingTime;
	};

	bool is_source_dead(const Source& source);


	class ParticleSystem
	{
		memory::ObjectPool<Particle> particles;
		memory::ObjectPool<graphics::Renderable> renderables;

		std::vector<Source> sources;

		real inverseMass;
		real damping;

		ParticleForceGenerator* forceGenerator;

		static const size_t PARTFREQ = 5;

		const graphics::MTexturedModel* model;
		const graphics::ShaderProgram*   shader;

	public:
		ParticleSystem(size_t maxParticles, real inverseMass, real damping, ParticleForceGenerator* pfg, const graphics::MTexturedModel* model, const graphics::ShaderProgram* shader ) :
			particles{ maxParticles }, renderables{ maxParticles }, sources(0), inverseMass(inverseMass), damping(damping), forceGenerator(pfg),
			model{ model }, shader{ shader }
		{
		}

		size_t getActiveParticles() const
		{
			return particles.getActiveElements();
		}

		const graphics::Renderable* getRenderable(size_t index) const
		{
			return &renderables.at(index);
		}

		void createSource(const glm::vec3& center, float sourceLifetime)
		{
			sources.push_back(Source{});
			sources.back().center = center;
			sources.back().sourceLifeTime = sourceLifetime;
			sources.back().startingTime = glfwGetTime();
		}

		// goes through all sources and activate new particles coming out of their center
		void createAndDestroy()
		{
			using namespace tmx::utils;

			// delete dead sources 
			sources.erase(std::remove_if(sources.begin(), sources.end(), is_source_dead), sources.end());

			for (size_t s = 0; s < sources.size(); s++)
			{
				for (size_t n = 0; n < PARTFREQ; n++)
				{
					glm::vec3 position = sources.at(s).center;
					glm::vec3 velocity{ 10*quick_random_number(), 10*quick_random_number(), 10*quick_random_number() };

					glm::vec3 rotation{ 0.0,1.0,0.0 };
					GLfloat	  angle = 360 * quick_random_number();
					GLfloat   scale = 1.0;
					
					try {
						particles.addBackElement();
						renderables.addBackElement();
					}
					catch (const std::out_of_range& oor)
					{
						std::cout << "too many particles" << std::endl;
					}


					particles.back().init(position, velocity, glm::vec3{ 0.0, 0.0, 0.0 }, inverseMass, damping, glm::vec3{ 0.0, 0.0, 0.0 });
					renderables.back().init(model, shader, position, rotation, angle, scale);
				}
			}
		}


		void integrateParticles(real duration)
		{
			for (size_t n = 0; n < particles.getActiveElements(); n++)
			{
				particles.at(n).integrate(duration);
				forceGenerator->updateForce(&particles.at(n), duration);
				particles.at(n).integrateVelocity(duration);

				renderables.at(n).position = particles.at(n).getPosition();
			}
		}

		void deactivateParticles()
		{
			for (size_t n = 0; n < particles.getActiveElements(); n++)
			{
				if (particles.at(n).getPosition().y <= 0.0)
				{
					particles.deleteElement(n);
					renderables.deleteElement(n);
				}
			}
		}


		const memory::ObjectPool<Particle>* getParticles()
		{
			return &particles;
		}

	};


	/*
	class ParticleSystem
	{
		memory::ObjectPool<Particle> particles;

		graphics::Renderable renderable;

		std::vector<Source> sources;


		real inverseMass;
		real damping;

		ParticleForceGenerator* forceGenerator;

		const graphics::MTexturedModel* model;
		const graphics::ShaderProgram*   shader;

		static const size_t PARTFREQ = 1;
	public:
		ParticleSystem(size_t maxParticles, real inverseMass, real damping, ParticleForceGenerator* pfg, const graphics::MTexturedModel* model, const graphics::ShaderProgram* shader) :
			particles{ maxParticles }, sources(0), inverseMass(inverseMass), damping(damping), forceGenerator(pfg),
			model{ model }, shader{ shader }
		{
			renderable.init(model, shader, glm::vec3{ 0.0,0.0,0.0 }, glm::vec3{ 0.0,1.0,0.0 }, 0.0, 1.0);
		}
		~ParticleSystem()
		{
			//delete[] pointersToPositions;
		}

		size_t getActiveParticles() const
		{
			return particles.getActiveElements();
		}

		const graphics::Renderable* getRenderable() const
		{
			return &renderable;
		}

		void createSource(const glm::vec3& center, float sourceLifetime)
		{
			sources.push_back(Source{});
			sources.back().center = center;
			sources.back().sourceLifeTime = sourceLifetime;
			sources.back().startingTime = glfwGetTime();
		}

		// goes through all sources and activate new particles coming out of their center
		void createAndDestroy()
		{
			using namespace tmx::utils;

			// delete dead sources 
			sources.erase(std::remove_if(sources.begin(), sources.end(), is_source_dead), sources.end());

			for (size_t s = 0; s < sources.size(); s++)
			{
				for (size_t n = 0; n < PARTFREQ; n++)
				{
					glm::vec3 position = sources.at(s).center;
					glm::vec3 velocity{ 100 * quick_random_number(), 100 * quick_random_number(), 100 * quick_random_number() };
					//glm::vec3 rotation{ 0.0,1.0,0.0 };
					//GLfloat	  angle = 0.0;
					//GLfloat   scale = 1.0;

					try {
						particles.addBackElement();
						//renderables.addBackElement();
					}
					catch (const std::out_of_range& oor)
					{
						std::cout << "too many particles" << std::endl;
					}


					particles.back().init(position, velocity, glm::vec3{ 0.0, 0.0, 0.0 }, inverseMass, damping, glm::vec3{ 0.0, 0.0, 0.0 });
					//renderables.back().init(model, shader,position, rotation, angle, scale);
				}
			}
		}


		void integrateParticles(real duration)
		{
			for (size_t n = 0; n < particles.getActiveElements(); n++)
			{
				particles.at(n).integrate(duration);
				forceGenerator->updateForce(&particles.at(n), duration);
				particles.at(n).integrateVelocity(duration);

				//renderables.at(n).position = particles.at(n).getPosition();
			}
		}

		void deactivateParticles()
		{
			for (size_t n = 0; n < particles.getActiveElements(); n++)
			{
				if (particles.at(n).getPosition().y <= 0.0)
				{
					particles.deleteElement(n);
					//renderables.deleteElement(n);
				}
			}
		}


		const memory::ObjectPool<Particle>* getParticles()
		{
			return &particles;
		}

	};

	*/


} } }
