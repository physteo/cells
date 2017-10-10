#include "LightDisplay.h"

namespace visuals {

	LightDisplay::LightDisplay(int width, int height, Box* box, PartSpecs* partSpecs, SystemTrajectory* trajectory, int level) :
		m_window{ "Coolattice", width, height },
		m_box{ box },
		m_camera(glm::vec3{ 0.0f,0.0f,1.0f }, glm::vec3{ 0.0f,0.0f,-1.0f }, glm::vec3{ 0.0f,1.0f,0.0f }, &m_window),
		m_renderer{ &m_camera },
		m_shader{},
		m_trajectory(trajectory),
		m_level(level),
		m_subBox{ (GLuint)width * 1.0 * m_box->getBoxCellLengthX() / m_box->getLx(), (GLuint)height * 1.0 * m_box->getBoxCellLengthY() / m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/subBox.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_renderablesPool{ m_box->getBoxCells().size() }
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
		//glm::mat4 projectionMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		glm::mat4 projectionMatrix = glm::ortho((GLfloat)0.0f, (GLfloat)width, (GLfloat)0.0f, (GLfloat)height, 0.2f, 50.f);
		m_shader.loadMatrix("projectionMatrix", projectionMatrix);
		//m_shader.loadVector("lightPosition", light.getPosition());
		//m_shader.loadVector("lightColour", light.getColour());
		//m_shader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		m_shader.stop();
		int numberOfSites = m_trajectory->at(0).size();
		m_renderables = new std::vector<tmx::graphics::Renderable>;
		m_renderables->resize(numberOfSites);


	}

	LightDisplay::~LightDisplay()
	{
		delete m_renderables;
	}

	void LightDisplay::run(int start, int end, int stride, int stepWait)
	{
		int delayer = 0;

		double lastTime = glfwGetTime();
		int nbFrames = 0;

		double lastTimeUpdate = glfwGetTime();
		int step = 0;
		while (!m_window.closed())
		{
			if (step < m_trajectory->size() - 1)
			{
				step++;
			}

			// Measure Speed
			double currentTime = glfwGetTime();
			nbFrames++;

			if (currentTime - lastTimeUpdate >= 0.01) {
				m_camera.move();
				lastTimeUpdate += 0.01;
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

			//double x, y;
			//m_window.getMousePosition(x, y);


			// construct Renderables
			fillRenderables(step);

			for (int i = 0; i < m_renderablesPool.getActiveElements(); i++)
			{
				m_renderer.submit(&m_renderablesPool.at(i));
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

	void LightDisplay::fillRenderables(int step)
	{

		m_renderablesPool.deleteAllElements();

		const CellColony& config = m_trajectory->at(step);

		for (int m = 0; m < m_box->getBoxCells().size(); m++)
		{
			bool fillSubBox;

			BoxCell& subBox = m_box->getBoxCells().at(m);
			Part* current = subBox.head.next;
			int counter = 0;
			while (current != nullptr)
			{
				counter++;
				current = current->next;
			}

		
			if (counter >= m_level) {
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



	}



}