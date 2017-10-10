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
#include "../GameComponent.h"
#include "../gameObjects/GameObject.h"

// utils
#include "../utils/quickRandom.h"

namespace tmx
{

	class Level
	{
	public:
		Level();
		~Level();
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

		// objects in the level
		// the vector of the gameComponents  holds pointers. This is not cachefriendly, but I need it for the moment
		// so that I can reallocate the vector without losing the pointers (and I need the pointers to the gameComponents...? where?)
		std::vector<tmx::GameComponent*>		gameComponents;
		// here I can create vectors of the objects itself because terrains will never be destroyed and also grass
		std::vector<tmx::graphics::Terrain>	terrains;
		std::vector<tmx::graphics::Renderable> grass;

		tmx::physics::ParticleWorld particleWorld;
		// create a "rendererWorld" also... it will take care of grouping renderables in the same batch/instance or something

	};


}

