#pragma once

// STL
#include <vector>

// graphics
#include "../../graphics/Window.h"
#include "../Camera.h"
#include "../ShaderProgram.h"
#include "../graphics/SimpleRenderer.h"
#include "../graphics/TerrainRenderer.h"
#include "../graphics/MTexturedModel.h"
#include "../graphics/Terrain.h"
#include "../graphics/Light.h"

// physics
#include "../physics/Pworld.h"


// game
#include "../gameObjects/GameObject.h"

// utils
#include "../utils/quickRandom.h"

namespace tmx
{

	class Level2
	{
	public:
		Level2();
		~Level2();
		inline sparky::graphics::Window& getWindow() { return window; }
		void runGame();

	private:
		void moveCamera();
		void startPhysicsFrame();
		void runPhysics(tmx::physics::real duration);
		void sinchronizeGameComponents();
		void submitToRenderers();
		void flushRenderers();
	private:

		sparky::graphics::Window window;
		tmx::graphics::Camera camera;

		// renderers
		tmx::graphics::SimpleRenderer	simpleRenderer;
		tmx::graphics::TerrainRenderer	terrainRenderer;

		// shaders
		tmx::graphics::StaticShader  shader;
		tmx::graphics::GrassShader   grassShader;
		tmx::graphics::TerrainShader terrainShader;

		// models and quads
		std::vector<tmx::graphics::MTexturedModel> models;
		std::vector<tmx::graphics::MTexturedQuad>  quads;

		
		gameObjects::TwoComponentsGameObjectVector<physics::Particle, graphics::Renderable> pgo;

		// here I can create vectors of the objects itself because terrains will never be destroyed and also grass
		std::vector<tmx::graphics::Terrain>	terrains;
		std::vector<tmx::graphics::Renderable> grass;

		tmx::physics::ParticleWorld particleWorld;
		// create a "rendererWorld" also... it will take care of grouping renderables in the same batch/instance or something

	};


}


