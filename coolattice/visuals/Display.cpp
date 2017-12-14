#include "Display.h"

namespace visuals {

	Display::Display(int width, int height, double adjustFactor, Box* box, PartSpecs* partSpecs, SystemTrajectory* trajectory, bool lightRendering) :
		m_defaultStage(0),
		m_window{ "Cells", width, height },
		m_box{box},
		m_camera(glm::vec3{ 0.0f,0.0f,1.0f }, glm::vec3{ 0.0f,0.0f,-1.0f }, glm::vec3{ 0.0f,1.0f,0.0f }, &m_window),
		m_renderer{ &m_camera },
		m_shader{},
		m_trajectory(trajectory),
		m_activeSite{ (GLuint) width * adjustFactor * partSpecs->getVectorOfPartTypes().at(m_defaultStage).getPartTypes().at(0).sig / m_box->getLx(), (GLuint) height * adjustFactor * partSpecs->getVectorOfPartTypes().at(m_defaultStage).getPartTypes().at(0).sig / m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/smile.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_inactiveSite{ (GLuint)width * adjustFactor * partSpecs->getVectorOfPartTypes().at(m_defaultStage).getPartTypes().at(partSpecs->getNumberOfTypes() - 1).sig / m_box->getLx(), (GLuint)height * adjustFactor * partSpecs->getVectorOfPartTypes().at(m_defaultStage).getPartTypes().at(partSpecs->getNumberOfTypes() - 1).sig / m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/redsiteCool.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_dividingCell{ (GLuint)width * adjustFactor * partSpecs->getVectorOfPartTypes().at(m_defaultStage).getPartTypes().at(partSpecs->getNumberOfTypes() - 1).sig / m_box->getLx(), (GLuint)height * adjustFactor * partSpecs->getVectorOfPartTypes().at(m_defaultStage).getPartTypes().at(partSpecs->getNumberOfTypes() - 1).sig / m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/redsiteSmall.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_subBox{ (GLuint)width * 0.9 * m_box->getBoxCellLengthX() / m_box->getLx(), (GLuint)height * 0.9 * m_box->getBoxCellLengthY()/ m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/subBox.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_play{ (GLuint)width * 0.05, (GLuint)height * 0.05,
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/play.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_stop{ (GLuint)width * 0.05, (GLuint)height * 0.05,
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/stop.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_renderablesPool{ lightRendering ? trajectory->maxCells() / 2 : (trajectory->maxCells() + m_box->getBoxCells().size())+1 },
		m_lightRendering(lightRendering)
	{

		//GLfloat skyR = 210 / 255.;
		//GLfloat skyG = 238 / 255.;
		//GLfloat skyB = 249 / 255.;
		//GLfloat skyR = 0 / 255.;
		//GLfloat skyG = 0 / 255.;
		//GLfloat skyB = 0 / 255.;
		GLfloat skyR = 250 / 255.;
		GLfloat skyG = 250 / 255.;
		GLfloat skyB = 250 / 255.;



		glClearColor(skyR, skyG, skyB, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// load stuff into the shader
		m_shader.start();
		GLenum error = glGetError();
		//glm::mat4 projectionMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.01f, 10.0f);
		glm::mat4 projectionMatrix = glm::ortho((GLfloat)0.0f, (GLfloat)width, (GLfloat)0.0f, (GLfloat)height, 0.2f, 50.f);
		m_shader.loadMatrix("projectionMatrix", projectionMatrix);
		//m_shader.loadVector("lightPosition", light.getPosition());
		//m_shader.loadVector("lightColour", light.getColour());
		//m_shader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		m_shader.stop();
		//int numberOfSites = m_trajectory->at(0).size();
		//m_renderables = new std::vector<tmx::graphics::Renderable>;
		//m_renderables->resize(numberOfSites);

		// playbackControls stuff
		pause = false;
		currentStep = 0;
		totalSteps = m_trajectory->size() - 1;
		jumpSteps = 1;
		mousePressedOnce = false;
		mousePressed = false;

		current_ix = 0.0;
		current_fx = m_window.getWidth();
		current_iy = 0.0;
		current_fy = m_window.getHeight();

		// playback stuff rendering

	}

	//Display::~Display()
	//{
	//	delete m_renderables;
	//}

	void Display::addSteps(size_t s)
	{
		if (currentStep + s < totalSteps)
		{
			currentStep += s;
		}
	}

	void Display::substractSteps(size_t s)
	{
		if (s <= currentStep )
		{
			currentStep -= s;
		}
	}


	void Display::moveCamera()
	{

		if (m_window.isKeyPressed(GLFW_KEY_ESCAPE))
		{
			m_shader.start();
			GLenum error = glGetError();
			glm::mat4 projectionMatrix = glm::ortho((GLfloat)0.0f, (GLfloat)m_window.getWidth(), (GLfloat)0.0f, (GLfloat)m_window.getHeight(), 0.2f, 50.f);
			m_shader.loadMatrix("projectionMatrix", projectionMatrix);
			m_shader.stop();
			mousePressedOnce = false;
		}

		if (m_window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (!mousePressedOnce)
			{
				m_window.getMousePosition(ix, iy);
				mousePressedOnce = true;
				mousePressed = true;

				current_ix = 0.0;
				current_fx = m_window.getWidth();
				current_iy = 0.0;
				current_fy = m_window.getHeight();

			}
			else
			{
				m_window.getMousePosition(fx, fy);
				
				std::cout << ix << ", " << iy << std::endl;
				std::cout << fx << ", " << fy << std::endl;


				m_shader.start();
				GLenum error = glGetError();
				glm::mat4 projectionMatrix = glm::ortho((GLfloat) (ix - m_window.getWidth()), (GLfloat)(fx - m_window.getWidth()),
														(GLfloat) (iy), (GLfloat)(fy),
														0.2f, 50.f);
				m_shader.loadMatrix("projectionMatrix", projectionMatrix);
				m_shader.stop();

				current_ix = ix;
				current_fx = fx;
				current_iy = iy;
				current_fy = fy;

				mousePressedOnce = false;
			}
		}



		
	}


	void Display::playbackControls()
	{

		//bool currentStep = 0;

		if (m_window.isKeyPressed(GLFW_KEY_SPACE))
		{
			pause = !pause;
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_0))
		{
			currentStep = 0;
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_1))
		{
			currentStep = (size_t) (totalSteps * 1.0/ 10.0);
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_2))
		{
			currentStep = (size_t)(totalSteps * 2.0 / 10.0);
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_3))
		{
			currentStep = (size_t)(totalSteps * 3.0 / 10.0);
			buttonPressed = true;
		}

		if (m_window.isKeyPressed(GLFW_KEY_4))
		{
			currentStep = (size_t)(totalSteps * 4.0 / 10.0);
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_5))
		{
			currentStep = (size_t)(totalSteps * 5.0 / 10.0);
			buttonPressed = true;

		}


		if (m_window.isKeyPressed(GLFW_KEY_6))
		{
			currentStep = (size_t)(totalSteps * 6.0 / 10.0);
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_7))
		{
			currentStep = (size_t)(totalSteps * 7.0 / 10.0);
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_8))
		{
			currentStep = (size_t)(totalSteps * 8.0 / 10.0);
			buttonPressed = true;

		}

		if (m_window.isKeyPressed(GLFW_KEY_9))
		{
			currentStep = (size_t)(totalSteps * 9.0 / 10.0);
			buttonPressed = true;
		}

		if (m_window.isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
		{
			if (m_window.isKeyPressed(GLFW_KEY_1)) {
				jumpSteps = 1;
				buttonPressed = true;
			}

			if (m_window.isKeyPressed(GLFW_KEY_5)) {
				jumpSteps = 5;
				buttonPressed = true;
			}

			if (m_window.isKeyPressed(GLFW_KEY_0)) {
				jumpSteps = 10;
				buttonPressed = true;
			}
		}

		if (m_window.isKeyPressed(GLFW_KEY_RIGHT))
		{
			addSteps(jumpSteps);
			buttonPressed = true;
		}

		if (m_window.isKeyPressed(GLFW_KEY_LEFT))
		{
			substractSteps(jumpSteps);
			buttonPressed = true;
		}

	}

	void Display::run(int start, int end, int stride, int stepWait)
	{
		buttonPressed = false;
		int delayer = 0;

		double lastTime = glfwGetTime();
		int nbFrames = 0;

		double lastTimeUpdate = glfwGetTime();
		double lastTimePressed = glfwGetTime();
		double lastTimeMousePressed = glfwGetTime();


		while (!m_window.closed())
		{
			if (currentStep < m_trajectory->size() - 1 && !pause)
			{
				currentStep++;
			}

			

			// Measure Speed
			double currentTime = glfwGetTime();
			nbFrames++;

			if (currentTime - lastTimeUpdate >= 0.01) {
				//m_camera.move();
				if (!buttonPressed) {
					playbackControls();
					lastTimePressed = glfwGetTime();
				}
				if (!mousePressed) {
					moveCamera();
					lastTimeMousePressed = glfwGetTime();
				}

				lastTimeUpdate += 0.01;
			}

			if (currentTime - lastTimePressed >= 0.1)
			{
				// can press again
				buttonPressed = false;
			}
			if (currentTime - lastTimeMousePressed >= 1.0)
			{
				// can press again
				mousePressed = false;
			}


			if (currentTime - lastTime >= 1.0) {
				std::cout << double(nbFrames) << "ms/frame" << std::endl;
				nbFrames = 0;
				lastTime += 1.0;
			}


			//std::cout << window.getWidth() << ", " << window.getHeight() << std::endl;
			try {
				m_window.clear();
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}

			double x, y;
			//m_window.getMousePosition(x, y);
			//std::cout << x << "," << y << std::endl;


			fillRenderables(currentStep);

			fillPlaybackRenderables(currentStep);


			for (int i = 0; i < m_renderablesPool.getActiveElements(); i++)
			{
				double leftX = m_renderablesPool.at(i).position.x - m_renderablesPool.at(i).texturedModel->getWidth() / 2.0;
				double rightX = m_renderablesPool.at(i).position.x + m_renderablesPool.at(i).texturedModel->getWidth() / 2.0;

				double highY = m_renderablesPool.at(i).position.y + m_renderablesPool.at(i).texturedModel->getHeight() / 2.0;
				double lowY = m_renderablesPool.at(i).position.y - m_renderablesPool.at(i).texturedModel->getHeight() / 2.0;

				//std::cout << leftX << std::endl;
				//std::cout << current_fx << std::endl;

				if (   leftX < current_fx  ) {
					//newinstanceRenderer.submit(psystem.getRenderable(n));
					m_renderer.submit(&m_renderablesPool.at(i));
				}

			}
			

			//for (int i = 0; i < m_renderables->size(); i++)
			//{
			//	m_renderer.submit(&m_renderables->at(i));
			//}

			m_renderer.flush();
			m_window.update();

			for (int d = 0; d < stepWait; d++)
			{
				delayer += d;
			}
			

		}
		std::cout << delayer << std::endl;

	}


	void Display::fillPlaybackRenderables(int step)
	{
		if (pause)
		{
			m_renderablesPool.addBackElement();
			tmx::graphics::Renderable& renderable = m_renderablesPool.back();

			double centerX = 2.0;
			double centerY = 2.0;

			renderable.init(&m_stop, &m_shader, glm::vec3{ (GLfloat)centerX * m_window.getWidth() / m_box->getLx() ,//+   0.5 * m_window.getWidth(),
				(GLfloat)centerY * m_window.getHeight() / m_box->getLy() ,//+   0.5 * m_window.getHeight(),
				(GLfloat) 0.0f },
				glm::vec3{ 0,1,0 },
				0.0f,
				1.0f);
		}
		else
		{
			m_renderablesPool.addBackElement();
			tmx::graphics::Renderable& renderable = m_renderablesPool.back();

			double centerX = 2.0;
			double centerY = 2.0;

			renderable.init(&m_play, &m_shader, glm::vec3{ (GLfloat)centerX * m_window.getWidth() / m_box->getLx() ,//+   0.5 * m_window.getWidth(),
				(GLfloat)centerY * m_window.getHeight() / m_box->getLy() ,//+   0.5 * m_window.getHeight(),
				(GLfloat) 0.0f },
				glm::vec3{ 0,1,0 },
				0.0f,
				1.0f);
		}




	}



	void Display::fillRenderables(int step)
	{

		m_renderablesPool.deleteAllElements();

		const CellColony& config = m_trajectory->at(step);

		if (!m_lightRendering){
			for (int m = 0; m < m_box->getBoxCells().size(); m++)
			{
				m_renderablesPool.addBackElement();
				tmx::graphics::Renderable& renderable = m_renderablesPool.back();
				Vector downLeftCorner = m_box->getBoxCells().at(m).downLeftCorner;
				Vector upperRightCorner = m_box->getBoxCells().at(m).upperRightCorner;

				double centerX = (downLeftCorner.x + upperRightCorner.x) / 2.0;
				double centerY = (downLeftCorner.y + upperRightCorner.y) / 2.0;

				renderable.init(&m_subBox, &m_shader, glm::vec3{ (GLfloat)centerX * m_window.getWidth() / m_box->getLx() ,//+   0.5 * m_window.getWidth(),
					(GLfloat)centerY * m_window.getHeight() / m_box->getLy() ,//+   0.5 * m_window.getHeight(),
					(GLfloat) 0.0f },
					glm::vec3{ 0,1,0 },
					0.0f,
					1.0f);

			}
		}

		for (int n = 0; n < config.size(); n++)
		{
			for (int p = 0; p < (m_lightRendering ? 1 : config.at(n).getNumOfParts()); p++)
			{
				const Part& part = config.at(n).getPart(p);
				Vector pbcPosition{part.position.x, part.position.y};
				m_renderablesPool.addBackElement();
				tmx::graphics::Renderable& renderable = m_renderablesPool.back();

				// map to the primitive box
				while (pbcPosition.x > m_box->getLx())
				{
					pbcPosition.x -= m_box->getLx();
				}
				while (pbcPosition.x <= 0.0)
				{
					pbcPosition.x += m_box->getLx();
				}
				
				while (pbcPosition.y > m_box->getLy())
				{
					pbcPosition.y -= m_box->getLy();
				}			
				while (pbcPosition.y <= 0.0)
				{
					pbcPosition.y += m_box->getLy();
				}

				if (part.type == 0) {
					if (part.currentStage == 2) {
						renderable.init(&m_dividingCell, &m_shader, glm::vec3{ (GLfloat)pbcPosition.x * m_window.getWidth() / m_box->getLx() ,//+   0.5 * m_window.getWidth(),
																		 (GLfloat)pbcPosition.y * m_window.getHeight() / m_box->getLy() ,//+   0.5 * m_window.getHeight(),
																		 (GLfloat) 0.0f },
							glm::vec3{ 0,1,0 },
							0.0f,
							1.0f);
					}
					else
					{
						renderable.init(&m_activeSite, &m_shader, glm::vec3{ (GLfloat)pbcPosition.x * m_window.getWidth() / m_box->getLx() ,//+   0.5 * m_window.getWidth(),
							(GLfloat)pbcPosition.y * m_window.getHeight() / m_box->getLy() ,//+   0.5 * m_window.getHeight(),
							(GLfloat) 0.0f },
							glm::vec3{ 0,1,0 },
							0.0f,
							1.0f);
					}

				}
				else if (!m_lightRendering)
				{
					if (part.type == 1 ) {
						renderable.init(&m_inactiveSite, &m_shader, glm::vec3{ (GLfloat)pbcPosition.x * m_window.getWidth() / m_box->getLx() ,//+   0.5 * m_window.getWidth(),
							(GLfloat)pbcPosition.y * m_window.getHeight() / m_box->getLy() ,//+   0.5 * m_window.getHeight(),
							(GLfloat) 0.0f },
							glm::vec3{ 0,1,0 },
							0.0f,
							1.0f);
					}
					else {
						assert(false);
					}
				}
			}
		}

	}



}