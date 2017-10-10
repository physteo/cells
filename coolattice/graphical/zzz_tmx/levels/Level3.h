#pragma once

// STL
#include <vector>

// graphics
#include "../../graphics/Window.h"
#include "../Camera.h"
#include "../ShaderProgram.h"
#include "../graphics/SimpleRenderer.h"
#include "../graphics/InstanceRenderer.h"
#include "../graphics/TerrainRenderer.h"
#include "../graphics/MTexturedModel.h"
#include "../graphics/Terrain.h"
#include "../graphics/Light.h"

// physics
#include "../physics/Pworld.h"
// particle system
#include "../physics/ParticleSystem/ParticleSystem.h"


// game
#include "../gameObjects/GameObject.h"

// utils
#include "../utils/quickRandom.h"


namespace tmx
{

	class Level3
	{
	public:
		Level3();
		~Level3();
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
		//tmx::graphics::MInstanceRenderer instanceRenderer;
		tmx::graphics::XInstanceRenderer newinstanceRenderer;


		// shaders
		tmx::graphics::StaticShader          shader;
		tmx::graphics::InstanceStaticShader  instanceShader;

		tmx::graphics::GrassShader   grassShader;
		tmx::graphics::GrassInstanceShader grassInstanceShader;

		tmx::graphics::TerrainShader terrainShader;

		// models and quads
		std::vector<tmx::graphics::MTexturedModel> models;
		std::vector<tmx::graphics::MTexturedQuad>  quads;


		gameObjects::TwoComponentsGameObjectVector<physics::Particle, graphics::Renderable> pgo;

		// here I can create vectors of the objects itself because terrains will never be destroyed and also grass
		std::vector<tmx::graphics::Terrain>	terrains;
		std::vector<tmx::graphics::GrassRenderable> grass;
		std::vector<const graphics::MTexturedModel*> pointersToLODModelsForGrass;

		tmx::physics::ParticleWorld particleWorld;
		// create a "rendererWorld" also... it will take care of grouping renderables in the same batch/instance or something

	};


}



