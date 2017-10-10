#include "Level3.h"
#include "../physics/Particle.h"

#include <glm/gtc/matrix_access.hpp>

namespace tmx {

	Level3::Level3() : window{ "Physgood", 960, 540 },
		//camera{ glm::vec3{ 18.0f,2.0f,-10.7f }, -4.0f, 346.0f, 0, &window },
		camera{ glm::vec3{ 10.0f,5.0f,10.0f }, glm::vec3{0.0,5.0,0.0}, glm::vec3{0.0f,1.0f,0.0f}, &window },
		simpleRenderer{ &camera },
		terrainRenderer{ &camera },
		//instanceRenderer{&camera},
		newinstanceRenderer{&camera},
		pgo{ 256 },
		particleWorld{ 1000U }
	{
		// in principle it should read from a file, which specifies the level.
		// we do it manually here

		GLfloat skyR = 210 / 255.;
		GLfloat skyG = 238 / 255.;
		GLfloat skyB = 249 / 255.;

		glClearColor(skyR, skyG, skyB, 1.0f);
		glEnable(GL_DEPTH_TEST);


		// create shaders

		// create Lights
		tmx::graphics::Light light{ glm::vec3{ 0.0f,50.0f,50.0f }, glm::vec3{ 1.0f,1.0f,1.0f } };

		// load stuff into the shaders
		shader.start();
		float fovy = 45.0f;
		float ratio = 16.0f / 9.0f;
		float near = 0.1f;
		float far = 1000.0f;

		glm::mat4 projectionMatrix = glm::perspective(fovy, ratio, near, far);
		camera.getFrustum().setCamInternals(fovy, ratio, near, far);
		//glm::mat4 projectionMatrix = glm::ortho(-16.f, 16.f, -9.f, 9.f, 0.2f, 50.f);


		shader.loadMatrix("projectionMatrix", projectionMatrix);
		shader.loadVector("lightPosition", light.getPosition());
		shader.loadVector("lightColour", light.getColour());
		shader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		shader.stop();

		instanceShader.start();
		instanceShader.loadMatrix("projectionMatrix", projectionMatrix);
		instanceShader.loadVector("lightPosition", light.getPosition());
		instanceShader.loadVector("lightColour", light.getColour());
		instanceShader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		instanceShader.stop();


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

		// load stuff into the grass shader
		grassInstanceShader.start();
		grassInstanceShader.loadMatrix("projectionMatrix", projectionMatrix);
		grassInstanceShader.loadVector("lightPosition", light.getPosition());
		grassInstanceShader.loadVector("lightColour", light.getColour());
		grassInstanceShader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		grassInstanceShader.stop();

		GLint level(0), border(0);

		tmx::graphics::MTexturedModel woodBallModel{ "C:/Users/Matteo/Documents/guscio.obj",
		{ "C:/Users/Matteo/Desktop/unspecified.png" }, tmx::graphics::BGR, tmx::graphics::RGB, level, border ,10.f,1.f };
		tmx::graphics::MTexturedModel cubeModel{ "C:/Users/Matteo/Documents/cube.obj",
		{ "C:/Users/Matteo/Pictures/cube.png" }, tmx::graphics::BGR, tmx::graphics::RGB, level, border,10.f,1.0f };
		glEnable(GL_BLEND);

		tmx::graphics::MTexturedQuad quadModel2D{ 10, 10,
		{ "C:/Users/Matteo/Pictures/niceGrass2.bmp" }, tmx::graphics::ABGR,  tmx::graphics::RGBA, level, border ,10.f,0.f,true };

		tmx::graphics::MTexturedQuad singleBlade{ 0.2f, 10.0f,
		{ "C:/Users/Matteo/Pictures/singleBlade.png" }, tmx::graphics::BGR,  tmx::graphics::RGB, level, border ,10.f,0.f,true };

		tmx::graphics::MTexturedQuad grassModelLOD0{ 2, 2,
		{ "C:/Users/Matteo/Pictures/textures/grass.png" }, tmx::graphics::BGRA,  tmx::graphics::RGBA, level, border ,10.f,0.f, true };

		tmx::graphics::MTexturedQuad grassModelLOD1{ 2, 2,
		{ "C:/Users/Matteo/Pictures/textures/grass_lod0.png" }, tmx::graphics::BGRA,  tmx::graphics::RGBA, level, border ,10.f,0.f, true };

		tmx::graphics::MTexturedQuad grassModelLOD2{ 2, 2,
		{ "C:/Users/Matteo/Pictures/textures/grass_lod1.png" }, tmx::graphics::BGRA,  tmx::graphics::RGBA, level, border ,10.f,0.f, true };


		models.push_back(std::move(woodBallModel));
		models.push_back(std::move(cubeModel));

		quads.push_back(std::move(quadModel2D));
		quads.push_back(std::move(singleBlade));

		// grass models
		quads.push_back(std::move(grassModelLOD0));
		quads.push_back(std::move(grassModelLOD1));
		quads.push_back(std::move(grassModelLOD2));

		pointersToLODModelsForGrass.push_back(&quads.back() - 2);
		pointersToLODModelsForGrass.push_back(&quads.back() - 1);
		pointersToLODModelsForGrass.push_back(&quads.back() - 0);
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
		//for(size_t i = 0; i < 10; i++)
		tmx::graphics::Terrain terrain1{ 0, 0, texturePaths4, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };
		tmx::graphics::Terrain terrain2{ 0,-1, texturePaths2, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };
		tmx::graphics::Terrain terrain3{ -1,0, texturePaths1, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };
		tmx::graphics::Terrain terrain4{ -1,-1, texturePaths3, &terrainShader, tmx::graphics::BGR, tmx::graphics::RGB, level, border, 10.f, 0.0f };

		terrains.push_back(terrain1);
		terrains.push_back(terrain2);
		terrains.push_back(terrain3);
		terrains.push_back(terrain4);


		grass.reserve(40000);
		//for (int i = 0; i < 40000; i++)
		//{
		//	GLfloat x = 200 * (tmx::utils::quick_random_number() - 0.5);
		//	GLfloat z = 200 * (tmx::utils::quick_random_number() - 0.5);
		//	GLfloat angle = 360 * tmx::utils::quick_random_number();
		//	//std::cout << x << std::endl;
		//
		//	grass.emplace_back(&quads.at(2), &pointersToLODModelsForGrass, &grassInstanceShader, glm::vec3{ x,quads.at(2).getHeight() / 2.0,z }, glm::vec3{ 0.0,1.0,0.0 }, angle, 1.0);
		//
		//}


	};


	Level3::~Level3()
	{

	}




	void Level3::runGame()
	{

		// position and shoot modes
		bool onHold = false;
		float timeCreation = 0.0f;
		float currentTimeCreation = 0.0f;
	
		enum { POSITION = 0, SHOOTING, NOMODE} mode = NOMODE;



		using namespace physics;
		using namespace graphics;

		size_t max = 30;

		// add some forces
		tmx::physics::ParticleGravity fgGravity{ glm::vec3{ 0.0f, -30.0f,0.0f } };
		tmx::physics::ParticleDrag    fgDrag{ 0.05f, 0.05f };

		// contact generators
		std::vector<tmx::physics::ParticleGroundContact*> groundContacts;
		std::vector<tmx::physics::ParticleSphere*> sphereContacts;
		std::vector<tmx::physics::ParticleRod*> rodContacts;


		Renderable testPart{ &models.at(0), &shader, glm::vec3{0.0f,0.0f,0.0f}, glm::vec3{0.0f,1.0f,0.0f}, 0.0f, 1.0f };
		glm::vec3 testPartVelocity{ 0.0f, 0.0f, 0.0f };

		std::vector<Renderable> testParts;
		testParts.reserve(100);
		bool created = true;
		glm::vec3 oldCreationPosition{0.0f, 0.0f, 0.0f};


		for (size_t i = 0; i < max; i++)
		{
			glm::vec3 position;
			if (i < max / 2.0)
				position = glm::vec3{ 8 * (tmx::utils::quick_random_number() - 0.5) - 20, 100 + 3 * i ,6 * (tmx::utils::quick_random_number() - 0.5) - 20 };
			else
				position = glm::vec3{ 8 * (tmx::utils::quick_random_number() - 0.5) - 27, 100 + 3 * (i - max / 2.0) + 10 ,6 * (tmx::utils::quick_random_number() - 0.5) - 27 };

			pgo.createGameObject();
			Particle* p = pgo.getPhys(i);
			p->setInverseMass(1.0);
			p->setPosition(position);
			p->setVelocity(glm::vec3{ 0, -10, 0 });
			p->setDamping(1.0);

			Renderable* r = pgo.getRend(i);
			r->position = position;
			r->shader = &shader;
			r->texturedModel = &models.at(0);
			r->scale = 0.1f;

			groundContacts.push_back(new ParticleGroundContact{ p, 2.0f, 0.7f });

			particleWorld.particles.push_back(p);
			particleWorld.registry.add(p, &fgGravity);
			particleWorld.registry.add(p, &fgDrag);
			particleWorld.contactGenerators.push_back(groundContacts[i]);
		}






		size_t counter = 0;
		for (int i = 0; i < max; i++) {
			for (int j = 0; j < i; j++) {
				Particle* pi = pgo.getPhys(i);
				Particle* pj = pgo.getPhys(j);

				sphereContacts.push_back(new ParticleSphere{ pi, pj, 2.0f, 2.0f, 1.0 });
				particleWorld.contactGenerators.push_back(sphereContacts.at(counter));
				counter++;
			}
		}



		// particle system test
		tmx::physics::ParticleGravity noForce{ glm::vec3{ 0.0f, 0.0f,0.0f } };

		particlesystem::ParticleSystem psystem{ 100000, 1.0, 1.0, &fgGravity, &models.at(0), &instanceShader};
		psystem.createSource(glm::vec3{ -20.0, 20.0,-30.0 }, 2000);


		particlesystem::ParticleSystem psystemGrass{ 100000, 1.0, 1.0, &fgGravity, &quads.at(0), &instanceShader };
		psystemGrass.createSource(glm::vec3{ 0.0, 30.0, 0.0 }, 2000);



		double lastTime = glfwGetTime();
		int nbFrames = 0;
		double lastTimeUpdate = glfwGetTime();


		

		// Game loop
		while (!window.closed())
		{
			window.clear();
			assert(testParts.size() < 101);
			if(created)
				testParts.clear();

			// Measure Speed
			double currentTime = glfwGetTime();
			nbFrames++;

			double elapsedTime = currentTime - lastTimeUpdate;
			if (elapsedTime >= 0.01) {
				lastTimeUpdate += elapsedTime;

				moveCamera();
				startPhysicsFrame();
				runPhysics(0.01);

				psystem.integrateParticles(0.01);
				psystem.deactivateParticles();

				psystemGrass.integrateParticles(0.01);
				psystemGrass.deactivateParticles();


			}


			sinchronizeGameComponents();



			//std::cout << window.getWidth() << ", " << window.getHeight() << std::endl;
			//player.move();
			double x, y;
			window.getMousePosition(x, y);
			//std::cout << (x - window.getWidth()/2.0)/window.getWidth() << ", " << (-y + window.getHeight() / 2.0)/window.getHeight() << std::endl;


			if (window.isKeyPressed(GLFW_KEY_P))
			{
				mode = POSITION;
			}
			if (window.isKeyPressed(GLFW_KEY_L))
			{
				mode = SHOOTING;
			}
			if (window.isKeyPressed(GLFW_KEY_DELETE))
			{
				mode = NOMODE;
			}


			if (mode == POSITION || mode == SHOOTING) {

				float xNearPlane = 2.0f * camera.getFrustum().m_width * (x - window.getWidth() / 2.0) / window.getWidth();
				float yNearPlane = 2.0f * camera.getFrustum().m_height * (-y + window.getHeight() / 2.0) / window.getHeight();
				float zNearPlane = camera.getFrustum().m_nearD;

				glm::vec3 eye = camera.getPosition();
				glm::vec3 center = camera.getCenter();
				glm::vec3 pointer;

				if (mode == POSITION) {
					glm::mat3 transf(0.0);
					transf[1] = camera.getFrustum().m_X;
					transf[0] = camera.getFrustum().m_Y;
					transf[2] = camera.getFrustum().m_Z;
					pointer = glm::normalize(transf * glm::vec3(yNearPlane, xNearPlane, zNearPlane));

					float finalDistance = (1.0f - eye.y) / pointer.y;
					glm::vec3 finalPosition = eye + finalDistance * glm::normalize(pointer);

					int numberOfPartsCreated = 100;
					float radiusCreation = 10.0f;
					if (created) {
						for (size_t n = 0; n < numberOfPartsCreated; n++)
						{
							glm::vec3 currentFinalPosition = finalPosition;
							float r = radiusCreation * utils::quick_random_number();
							float theta = 360.0f * utils::quick_random_number();
							currentFinalPosition.x += r * cos(theta);
							currentFinalPosition.z += r * sin(theta);
							//currentFinalPosition.x += radiusCreation * (utils::quick_random_number() - 0.5f);
							//currentFinalPosition.z += radiusCreation * (utils::quick_random_number() - 0.5f);
							float angle = 360.0 * utils::quick_random_number();
							testParts.emplace_back(&quads.at(2), &shader, currentFinalPosition, glm::vec3{ 0.0f,1.0f,0.0f }, angle, 1.0f);
							//std::cout << testPart.position.x << ", " << testPart.position.y << ", " << testPart.position.z << std::endl;
						}
					}
					else {
						// translate along
						for (size_t n = 0; n < testParts.size(); n++)
						{
							testParts.at(n).position += finalPosition - oldCreationPosition;
						}
						oldCreationPosition = finalPosition;
					}

					for (size_t n = 0; n < testParts.size(); n++) {
						simpleRenderer.submit(&testParts.at(n));
					}
				}

				else if (mode == SHOOTING)
				{
					pointer = center;
					testPart.position = pointer;
					testPartVelocity = 500.0f * glm::normalize(center - eye);
					std::cout << testPart.position.x << ", " << testPart.position.y << ", " << testPart.position.z << std::endl;
				}



				currentTimeCreation = glfwGetTime();
				if (onHold && (currentTimeCreation - timeCreation) > 0.25)
				{
					onHold = false;
				}

				if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && (!onHold))
				{
					created = true;
					std::cout << "Pressed" << std::endl;

					if (mode == POSITION) {
						for (size_t n = 0; n < testParts.size(); n++) {
							glm::vec3 position = testParts.at(n).position;
							//pgo.createGameObject();
							//Particle* p = pgo.getPhys(pgo.size() - 1);
							//p->setInverseMass(1.0);
							//p->setPosition(position);
							//p->setVelocity(testPartVelocity);
							//p->setDamping(1.0);

							//Renderable* r = pgo.getRend(pgo.size() - 1);
							//r->position = position;
							//r->shader = &shader;
							//r->texturedModel = &models.at(0);
							//r->scale = 0.1f;

							//groundContacts.push_back(new ParticleGroundContact{ p, 2.0f, 0.7f });
							//
							//particleWorld.particles.push_back(p);
							//particleWorld.registry.add(p, &fgGravity);
							//particleWorld.registry.add(p, &fgDrag);
							//particleWorld.contactGenerators.push_back(groundContacts.back());

							// for grass
							grass.emplace_back(&quads.at(2), &pointersToLODModelsForGrass, &grassInstanceShader, position, glm::vec3{ 0.0,1.0,0.0 }, testParts.at(n).angle, 1.0);
						}
					}

					// counter is untile size - 1 because the particle should not interact with itself
					//for (int i = 0; i < pgo.size() - 1; i++)
					//{
					//	Particle* pj = p;
					//	Particle* pi = pgo.getPhys(i);
					//
					//	sphereContacts.push_back(new ParticleSphere{ pj, pi, 2.0f, 2.0f, 1.0 });
					//	particleWorld.contactGenerators.push_back(sphereContacts.back());
					//}
					onHold = true;
					timeCreation = glfwGetTime();

				}
				else
				{
					created = false;
				}


			}




			submitToRenderers();
			
			for (size_t n = 0; n < psystem.getActiveParticles(); n++) {
				if (camera.getFrustum().pointInFrustum(psystem.getParticles()->at(n).getPosition()) == 2) {
					newinstanceRenderer.submit(psystem.getRenderable(n));
				}
			}
			
			for (size_t n = 0; n < psystemGrass.getActiveParticles(); n++) {
				if (camera.getFrustum().pointInFrustum(psystemGrass.getParticles()->at(n).getPosition()) == 2) {
					newinstanceRenderer.submit(psystemGrass.getRenderable(n));
				}
			}

			//glm::vec3 camPos = camera.getPosition();
			//graphics::Renderable object{ &models.at(0), &shader, glm::vec3{0.0,40.0,0.0}, glm::vec3{ 0.0,1.0,0.0 }, 0.0, 1.0 };
			//simpleRenderer.submit(&object);

			//instanceRenderer.submit(psystem.getRenderable());
			//if(psystem.getActiveParticles() > 0)
			//	instanceRenderer.getInstancedData(psystem.getParticles());

			flushRenderers();

			if (currentTime - lastTime >= 1.0) {
				std::cout << double(nbFrames) << "fps" << std::endl;
				nbFrames = 0;
				lastTime += 1.0;
			}



			if (x >= 6000) {
				psystem.createAndDestroy();
				psystemGrass.createAndDestroy();
			}
			if (y >= 3000) {
				std::cout << psystem.getActiveParticles() + psystemGrass.getActiveParticles() << std::endl;
			}

			// level logic: create new objects, destroy others
			if (x >= 2000) {
				float random = tmx::utils::quick_random_number();
				if (random < 0.9 && pgo.size() < 50000) {
					float x = 100 * tmx::utils::quick_random_number();
					float y = 100 * (1 + tmx::utils::quick_random_number());
					float z = 30 * (tmx::utils::quick_random_number() - 0.5);

					glm::vec3 position{ x, y, z };
					// create
					pgo.createGameObject();
					Particle* p = pgo.getPhys(pgo.size() - 1);
					p->setInverseMass(1.0);
					p->setPosition(position);
					p->setVelocity(glm::vec3{ 0, -10, 0 });
					p->setDamping(1.0);
					p->unselect();

					Renderable* r = pgo.getRend(pgo.size() - 1);
					r->position = position;
					r->shader = &shader;
					r->texturedModel = &models.at(0);
					r->unselect();

					groundContacts.push_back(new ParticleGroundContact{ p, 2.0f, 0.7f });

					particleWorld.particles.push_back(p);
					particleWorld.registry.add(p, &fgGravity);
					particleWorld.registry.add(p, &fgDrag);
					particleWorld.contactGenerators.push_back(groundContacts.back());

					for (size_t j = 0; j < pgo.size() - 1; j++)
					{
						Particle* pj = pgo.getPhys(j);
						sphereContacts.push_back(new ParticleSphere{ p, pj, 2.0f, 2.0f, 1.0 });
						particleWorld.contactGenerators.push_back(sphereContacts.back());
					}


				}
				else
				{
					// destroy
					if (pgo.size() > 0)
						pgo.selectObject((size_t)(random * (pgo.size())));
				}
				
			}

			//finalize destruction
			particleWorld.deleteSelected();
			pgo.destroySelectedObjects();


			//std::cout << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << std::endl;

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




	void Level3::moveCamera()
	{
		camera.move();
	}

	void Level3::startPhysicsFrame()
	{
		particleWorld.startFrame();
	}

	void Level3::runPhysics(tmx::physics::real duration)
	{
		particleWorld.runPhysics(duration);
	}

	void Level3::sinchronizeGameComponents()
	{
		using namespace physics;
		using namespace graphics;
		for (int i = 0; i < pgo.size(); i++)
		{
			Particle* p = pgo.getPhys(i);
			Renderable* r = pgo.getRend(i);
			r->position = p->getPosition();
		}
	}

	void Level3::submitToRenderers()
	{


		// submit all the renderables 
		for (int i = 0; i < pgo.size(); i++)
		{

			if (camera.getFrustum().pointInFrustum(pgo.getRend(i)->position) == 2) {
				simpleRenderer.submit(pgo.getRend(i));
			}
		}

		for (int i = 0; i < terrains.size(); i++)
		{
			//if (camera.getFrustum().pointInFrustum(terrains.at(i).position) == 2)
			simpleRenderer.submit(&terrains.at(i));
		}
		for (int i = 0; i < grass.size(); i++)
		{
			graphics::GrassRenderable& grassRenderable = grass.at(i);
			const glm::vec3& pos = grassRenderable.position;

			float distance2 = glm::length2(camera.getPosition() - pos);
			
			float distanceLOD1 = 20.0f;
			float distanceLOD2 = 40.0f;
			
			float maxDistance = 125.0f;

			if (camera.getFrustum().pointInFrustum(pos) == 2 && distance2 < maxDistance * maxDistance)
			{

				if (distance2 > distanceLOD2 * distanceLOD2) {
					grassRenderable.texturedModel = grassRenderable.LODtexturedModels->at(0);
					newinstanceRenderer.submit(&grass.at(i));
				} 
				else if (distance2 > distanceLOD1 * distanceLOD1 && distance2 < distanceLOD2 * distanceLOD2) {
					grassRenderable.texturedModel = grassRenderable.LODtexturedModels->at(0);
					newinstanceRenderer.submit(&grass.at(i));
				}
				else {
					grassRenderable.texturedModel = grassRenderable.LODtexturedModels->at(0);
					newinstanceRenderer.submit(&grass.at(i));
				}

			}
		}
		//for (int i = 0; i < grass.size(); i++)
		//{
		//	glm::vec3 posCam = camera.getPosition();
		//	glm::vec3 posGrass = grass.at(i).position;
		//	float distance2 = glm::length2(posCam - posGrass);
		//	float maxDistance = 75.0f;
		//	if ( distance2 < maxDistance * maxDistance) {
		//		newinstanceRenderer.submit(&grass.at(i));
		//	}
		//}
	}


	void Level3::flushRenderers()
	{
		terrainRenderer.flush();
		simpleRenderer.flush();
		newinstanceRenderer.flush();
	}

}