#include "Level.h"
#include "../physics/Particle.h"
namespace tmx {

	Level::Level() : window{ "Physgood", 960, 540 },
		//camera{ glm::vec3{ 18.0f,-2.0f,-10.7f }, -4.0f, 346.0f, 0, &window },
		camera{ glm::vec3{ 1.0f,1.0f,1.0f }, glm::vec3{ 0.0,0.0,0.0 }, glm::vec3{ 0.0f,1.0f,0.0f }, &window },

		simpleRenderer{ &camera },
		terrainRenderer{ &camera },
		particleWorld{ 1000U }
	{
		// in principle it should read from a file, which specifies the level.
		// we do it manually here
		 
		GLfloat skyR = 210 / 255.;
		GLfloat skyG = 238 / 255.;
		GLfloat skyB = 249 / 255.;

		glClearColor(skyR, skyG, skyB, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// create shaders

		// create Lights
		tmx::graphics::Light light{ glm::vec3{ 0.0f,50.0f,50.0f }, glm::vec3{ 1.0f,1.0f,1.0f } };

		// load stuff into the shaders
		shader.start();
		float fovy  = 45.0f;
		float ratio = 16.0f / 9.0f;
		float near  = 0.1f;
		float far   = 1000.0f;

		glm::mat4 projectionMatrix = glm::perspective(fovy, ratio, near, far);
		camera.getFrustum().setCamInternals(fovy, ratio, near, far);

		//glm::mat4 projectionMatrix = glm::ortho(-16.f, 16.f, -9.f, 9.f, 0.2f, 50.f);
		shader.loadMatrix("projectionMatrix", projectionMatrix);
		shader.loadVector("lightPosition", light.getPosition());
		shader.loadVector("lightColour", light.getColour());
		shader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		shader.stop();

		// load stuff into the terrain shader
		terrainShader.start();
		terrainShader.loadMatrix("projectionMatrix", projectionMatrix);
		terrainShader.loadVector("lightPosition", light.getPosition());
		terrainShader.loadVector("lightColour", light.getColour());
		terrainShader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		terrainShader.stop();

		// load stuff into the grass shader
		grassShader.start();
		grassShader.loadMatrix("projectionMatrix", projectionMatrix);
		grassShader.loadVector("lightPosition", light.getPosition());
		grassShader.loadVector("lightColour", light.getColour());
		grassShader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		grassShader.stop();


		GLint level(0), border(0);

		tmx::graphics::MTexturedModel woodBallModel{ "C:/Users/Matteo/Documents/guscio.obj",
		{ "C:/Users/Matteo/Desktop/unspecified.png" }, tmx::graphics::BGR, tmx::graphics::RGB, level, border ,10.f,1.f };
		tmx::graphics::MTexturedModel cubeModel{ "C:/Users/Matteo/Documents/cube.obj",
		{ "C:/Users/Matteo/Pictures/cube.png" }, tmx::graphics::BGR, tmx::graphics::RGB, level, border,10.f,1.0f };
		glEnable(GL_BLEND);
		tmx::graphics::MTexturedQuad quadModel2D{ 10, 10,
		{ "C:/Users/Matteo/Pictures/niceGrass.bmp" }, tmx::graphics::ABGR,  tmx::graphics::RGBA, level, border ,10.f,0.f,true };

		models.push_back(std::move(woodBallModel));
		models.push_back(std::move(cubeModel));
		quads.push_back(std::move(quadModel2D));

		// **** Terrain ****

		// prepare terrain textures
		std::vector<std::string> texturePaths = { "C:/Users/Matteo/Pictures/grass.png",
			//"C:/Users/Matteo/Pictures/textures/winter/111_tiled.png",
			"C:/Users/Matteo/Pictures/stones.png",
			//"C:/Users/Matteo/Pictures/textures/winter/114_strada.png",
			//"C:/Users/Matteo/Pictures/textures/winter/112_grass.png",
			"C:/Users/Matteo/Pictures/sand.png",
			"C:/Users/Matteo/Pictures/flowers.png" };
		//"C:/Users/Matteo/Pictures/blendMap1.png" };
		std::vector<std::string> texturePaths1 = texturePaths; texturePaths1.push_back("C:/Users/Matteo/Pictures/blendMap1.png");
		std::vector<std::string> texturePaths2 = texturePaths; texturePaths2.push_back("C:/Users/Matteo/Pictures/blendMap2.png");
		std::vector<std::string> texturePaths3 = texturePaths; texturePaths3.push_back("C:/Users/Matteo/Pictures/blendMap3.png");
		std::vector<std::string> texturePaths4 = texturePaths; texturePaths4.push_back("C:/Users/Matteo/Pictures/blendMap4.png");

		// create Terrains and store them
		tmx::graphics::Terrain terrain1{ 0, 0, texturePaths4, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };
		tmx::graphics::Terrain terrain2{ 0,-1, texturePaths2, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };
		tmx::graphics::Terrain terrain3{ -1,0, texturePaths1, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };
		tmx::graphics::Terrain terrain4{ -1,-1, texturePaths3, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };

		terrains.push_back(terrain1);
		terrains.push_back(terrain2);
		terrains.push_back(terrain3);
		terrains.push_back(terrain4);


		grass.reserve(200);
		for (int i = 0; i < 200; i++)
		{
			GLfloat x = 200 * (tmx::utils::quick_random_number() - 0.5);
			GLfloat z = 200 * (tmx::utils::quick_random_number() - 0.5);
			GLfloat angle = 360 * tmx::utils::quick_random_number();
			std::cout << x << std::endl;

			grass.emplace_back(&quads.at(0), &grassShader, glm::vec3{ x,quadModel2D.getHeight() / 2.0,z }, glm::vec3{ 0.0,1.0,0.0 }, angle, 1.0);
		}

		/////tmx::graphics::Player player{ &window, &woodBallModel, &shader, glm::vec3{0,1,0}, glm::vec3{0,1,0}, 0.0, 1.0 };


		// physics initialization stuff
/*
		// basic two-particle interacting 
		glm::vec3 position1{ -5.0f, 20.0f, 0.0f };
		glm::vec3 velocity1{ -50.0f, -3.0f, -0.0f };
		glm::vec3 position2{ +5.0f, 20.0f, 0.0f };
		glm::vec3 velocity2{ +50.0f, 0.0f, +0.0f };


		tmx::GameComponent testGameComponent1{	new tmx::graphics::Renderable{ &models.at(0), &shader },
												new tmx::physics::Particle{ position1, velocity1,{}, 1.0f,1.0f,{} },
												position1 };
		tmx::GameComponent testGameComponent2{	new tmx::graphics::Renderable{ &models.at(0), &shader },
												new tmx::physics::Particle{ position2, velocity2,{}, 1.0f,1.0f,{} },
												position1 };
		// player
		//p/tmx::graphics::Renderable testRenderablePlayer	{ &models.at(0),		&shader };
		//p/tmx::physics::Particle testPartPlayer	{ glm::vec3{ 0.0,0.0,0.0 },{},{}, 1.0f,1.0f,{} };
		//p/tmx::GameComponentPlayer testGameComponentPlayer{ &testRenderablePlayer, &testPartPlayer, glm::vec3{ 0.0,0.0,0.0 }, window };

		// physics stuff
		tmx::physics::ParticleForceRegistry forceRegistry;

		particleWorld.registry = forceRegistry;

		particleWorld.particles.push_back(testGameComponent1.getParticle());
		particleWorld.particles.push_back(testGameComponent2.getParticle());
		//p/particleWorld.particles.push_back(testGameComponentPlayer.getParticle());

		// add some forces
		tmx::physics::ParticleGravity fgGravity{ glm::vec3{ 0.0f, -30.0f,0.0f } };
		tmx::physics::ParticleSpring  fgSpring1{ &testPart2, 10.0f, 0.0f };
		tmx::physics::ParticleSpring  fgSpring2{ &testPart1, 10.0f, 0.0f };

		particleWorld.registry.add(testGameComponent1.getParticle(), &fgSpring1);
		particleWorld.registry.add(testGameComponent2.getParticle(), &fgSpring2);
		particleWorld.registry.add(testGameComponent1.getParticle(), &fgGravity);
		particleWorld.registry.add(testGameComponent2.getParticle(), &fgGravity);
		//p/particleWorld.registry.add(testGameComponentPlayer.getParticle(), &fgGravity);

		// add some contacts
		tmx::physics::ParticleCable cable{ testGameComponent1.getParticle(), testGameComponent2.getParticle(),  8.0f, 1.0f };
		tmx::physics::ParticleSphere volumeExclusion12{ testGameComponent1.getParticle(), testGameComponent2.getParticle(), 2.0f, 2.0f, 1.0 };
		//p/tmx::physics::ParticleSphere volumeExclusion1Player{ testGameComponent1.getParticle(), testGameComponentPlayer.getParticle(), 2.0f, 2.0f, 1.0 };
		//p/tmx::physics::ParticleSphere volumeExclusion2Player{ testGameComponent2.getParticle(), testGameComponentPlayer.getParticle(), 2.0f, 2.0f, 1.0 };

		tmx::physics::ParticleGroundContact groundContact1{ testGameComponent1.getParticle(), 2.0f, .02f };
		tmx::physics::ParticleGroundContact groundContact2{ testGameComponent2.getParticle(), 2.0f, .02f };
		//p/tmx::physics::ParticleGroundContact groundContactPlayer{ testGameComponentPlayer.getParticle(), 2.0f, .02f };

		particleWorld.contactGenerators.push_back(&cable);
		particleWorld.contactGenerators.push_back(&volumeExclusion12);
		//p/particleWorld.contactGenerators.push_back(&volumeExclusion1Player);
		//p/particleWorld.contactGenerators.push_back(&volumeExclusion2Player);
		particleWorld.contactGenerators.push_back(&groundContact1);
		particleWorld.contactGenerators.push_back(&groundContact2);
		//p/particleWorld.contactGenerators.push_back(&groundContactPlayer);

		tmx::physics::ParticleDrag    fgDrag{ 0.05f, 0.05f };
		//
		//
		//forceRegistry.add(testGameComponent1.getParticle(), &fgGravity);
		//forceRegistry.add(testGameComponent1.getParticle(), &fgSpring1);
		//forceRegistry.add(testGameComponent2.getParticle(), &fgSpring2);
		//forceRegistry.add(testGameComponent1.getParticle(), &fgDrag);
		//forceRegistry.add(testGameComponent2.getParticle(), &fgDrag);



		// group of particles

		std::vector<tmx::graphics::Renderable*> renderables;
		std::vector<tmx::physics::Particle*> particles;
		std::vector<tmx::GameComponent*> GameComponents;
		std::vector<tmx::physics::ParticleGroundContact*> groundContacts;
		std::vector<tmx::physics::ParticleSphere*> sphereContacts;
		std::vector<tmx::physics::ParticleRod*> rodContacts;

		size_t max = 50;

		for (int i = 0; i < max; i++) {
			glm::vec3 position;
			if (i < max / 2.0)
				position = glm::vec3{ 8 * (tmx::utils::quick_random_number() - 0.5) - 20, 100 + 3 * i ,6 * (tmx::utils::quick_random_number() - 0.5) - 20 };
			else
				position = glm::vec3{ 8 * (tmx::utils::quick_random_number() - 0.5) - 27, 100 + 3 * (i - max / 2.0) + 10 ,6 * (tmx::utils::quick_random_number() - 0.5) - 27 };
			glm::vec3 velocity{ 0,-10,0 };
			renderables.push_back(new tmx::graphics::Renderable{ &woodBallModel, &shader });
			particles.push_back(new tmx::physics::Particle{ position, velocity, glm::vec3{}, 1.0f, 1.0f, glm::vec3{} });
			GameComponents.push_back(new tmx::GameComponent{ renderables.at(i), particles.at(i), position });
			groundContacts.push_back(new tmx::physics::ParticleGroundContact{ GameComponents.at(i)->getParticle(), 2.0f, 0.7f });

			particleWorld.particles.push_back(GameComponents.at(i)->getParticle());
			particleWorld.registry.add(GameComponents.at(i)->getParticle(), &fgGravity);
			particleWorld.registry.add(GameComponents.at(i)->getParticle(), &fgDrag);
			particleWorld.contactGenerators.push_back(groundContacts[i]);
		}

		int counter = 0;
		for (int i = 0; i < max; i++) {
			for (int j = 0; j < i; j++) {
				sphereContacts.push_back(new tmx::physics::ParticleSphere{ GameComponents.at(i)->getParticle(), GameComponents.at(j)->getParticle(), 2.0f, 2.0f, 1.0 });
				particleWorld.contactGenerators.push_back(sphereContacts.at(counter));
				counter++;
			}
		}

		//rodContacts.emplace_back(GameComponents.at(0).getParticle(), GameComponents.at(3).getParticle(), fabs(GameComponents.at(0).getParticle() - GameComponents.at(3).getParticle()));
		//particleWorld.contactGenerators.push_back(&(rodContacts.at(0)));
*/
	};


	Level::~Level()
	{
		for (int i = 0; i < gameComponents.size(); i++)
		{
			//delete gameComponents.at(i);
		}
	}

	bool is_go_selected(const GameComponent* gc)
	{
		return gc->getParticle()->isSelected();
	}

	void Level::runGame()
	{
		using namespace physics;
		using namespace graphics;

		size_t max = 1000;
		// add some forces
		tmx::physics::ParticleGravity fgGravity{ glm::vec3{ 0.0f, -30.0f,0.0f } };
		tmx::physics::ParticleDrag    fgDrag{ 0.05f, 0.05f };

		// contact generators
		std::vector<tmx::physics::ParticleGroundContact*> groundContacts;
		std::vector<tmx::physics::ParticleSphere*> sphereContacts;
		std::vector<tmx::physics::ParticleRod*> rodContacts;





		// group of particles



		for (int i = 0; i < max; i++) {
			glm::vec3 position;
			if (i < max / 2.0)
				position = glm::vec3{ 8 * (tmx::utils::quick_random_number() - 0.5) - 20, 100 + 3 * i ,6 * (tmx::utils::quick_random_number() - 0.5) - 20 };
			else
				position = glm::vec3{ 8 * (tmx::utils::quick_random_number() - 0.5) - 27, 100 + 3 * (i - max / 2.0) + 10 ,6 * (tmx::utils::quick_random_number() - 0.5) - 27 };


			glm::vec3 velocity{ 0,-10,0 };
			
			gameComponents.push_back(new tmx::GameComponent{ new tmx::graphics::Renderable{ &models.at(0), &shader },
															 new tmx::physics::Particle{ position, velocity, glm::vec3{}, 1.0f, 1.0f, glm::vec3{} },
															position });

			groundContacts.push_back(new tmx::physics::ParticleGroundContact{ gameComponents.at(i)->getParticle(), 2.0f, 0.7f });

			particleWorld.particles.push_back(gameComponents.at(i)->getParticle());
			particleWorld.registry.add(gameComponents.at(i)->getParticle(), &fgGravity);
			particleWorld.registry.add(gameComponents.at(i)->getParticle(), &fgDrag);
			particleWorld.contactGenerators.push_back(groundContacts[i]);
		}

		int counter = 0;
		for (int i = 0; i < max; i++) {
			for (int j = 0; j < i; j++) {
				sphereContacts.push_back(new tmx::physics::ParticleSphere{ gameComponents.at(i)->getParticle(), gameComponents.at(j)->getParticle(), 2.0f, 2.0f, 1.0 });
				particleWorld.contactGenerators.push_back(sphereContacts.at(counter));
				counter++;
			}
		}
		





		double lastTime = glfwGetTime();
		int nbFrames = 0;
		double lastTimeUpdate = glfwGetTime();

		// Game loop
		while (!window.closed())
		{
			window.clear();

			// Measure Speed
			double currentTime = glfwGetTime();
			nbFrames++;


			if (currentTime - lastTimeUpdate >= 0.01) {
				lastTimeUpdate += 0.01;
				moveCamera();
				startPhysicsFrame();
				runPhysics(0.01);
			}


			sinchronizeGameComponents();

			//submitToRenderers();
			//
			//flushRenderers();

			if (currentTime - lastTime >= 1.0) {
				std::cout << double(nbFrames) << "ms/frame" << std::endl;
				nbFrames = 0;
				lastTime += 1.0;
			}

			//std::cout << window.getWidth() << ", " << window.getHeight() << std::endl;
			//player.move();
			double x, y;
			window.getMousePosition(x, y);
			

			// level logic: create new objects, destroy others

			if (x > 500) {
				for (size_t i = 0; i < gameComponents.size(); i++) {

				}
			}

			// level logic: create new objects, destroy others
			if (x >= 600 ) {
				float random = tmx::utils::quick_random_number();
				if (random < 0.9 && gameComponents.size() < 50000 ) {
					float x = 100 * tmx::utils::quick_random_number();
					float y = 100 * (1 + tmx::utils::quick_random_number());
					float z = 30 * (tmx::utils::quick_random_number() - 0.5);
					
					// create
					glm::vec3 position{ x, y, z };
					glm::vec3 velocity{ 0,-10,0 };

					gameComponents.push_back(new tmx::GameComponent{ new tmx::graphics::Renderable{ &models.at(0), &shader },
						new tmx::physics::Particle{ position, velocity, glm::vec3{}, 1.0f, 1.0f, glm::vec3{} },
						position });

					groundContacts.push_back(new tmx::physics::ParticleGroundContact{ gameComponents.back()->getParticle(), 2.0f, 0.7f });

					particleWorld.particles.push_back(gameComponents.back()->getParticle());
					particleWorld.registry.add(gameComponents.back()->getParticle(), &fgGravity);
					particleWorld.registry.add(gameComponents.back()->getParticle(), &fgDrag);
					particleWorld.contactGenerators.push_back(groundContacts.back());

					for (size_t j = 0; j < gameComponents.size() - 1; j++)
					{
						Particle* pj = gameComponents.at(j)->getParticle();
						sphereContacts.push_back(new ParticleSphere{ gameComponents.back()->getParticle(), pj, 2.0f, 2.0f, 1.0 });
						particleWorld.contactGenerators.push_back(sphereContacts.back());
					}


				}
				else
				{
					if (gameComponents.size() > 0) {
						physics::Particle* p = gameComponents.at((size_t)(random * gameComponents.size()))->getParticle();
						p->select();
					}
				}
				std::cout << gameComponents.size() << std::endl;
			}



			//finalize destruction
			particleWorld.deleteSelected();

			if(gameComponents.size() > 0)
				gameComponents.erase(std::remove_if(gameComponents.begin(), gameComponents.end(), is_go_selected), gameComponents.end());


			window.update();
		}


		// clean

		for (size_t i = 0; i < groundContacts.size(); i++)
		{
			delete groundContacts.at(i);
		}

		for (size_t i = 0; i < sphereContacts.size(); i++)
		{
			delete sphereContacts.at(i);
		}

		for (size_t i = 0; i < rodContacts.size(); i++)
		{
			delete rodContacts.at(i);
		}


	}




	void Level::moveCamera()
	{
		camera.move();
	}

	void Level::startPhysicsFrame()
	{
		particleWorld.startFrame();
	}

	void Level::runPhysics(tmx::physics::real duration)
	{
		particleWorld.runPhysics(duration);
	}

	void Level::sinchronizeGameComponents()
	{
		for (int i = 0; i < gameComponents.size(); i++)
		{
			gameComponents.at(i)->sinchronize();
		}
	}

	void Level::submitToRenderers()
	{


		// submit all the renderables 
		for (int i = 0; i < gameComponents.size(); i++)
		{
			simpleRenderer.submit(gameComponents.at(i)->getRenderable());
		}

		for (int i = 0; i < terrains.size(); i++)
		{
			simpleRenderer.submit(&terrains.at(i));
		}

		for (int i = 0; i < grass.size(); i++)
		{
			simpleRenderer.submit(&grass.at(i));
		}
	}


	void Level::flushRenderers()
	{
		terrainRenderer.flush();
		simpleRenderer.flush();
	}

}