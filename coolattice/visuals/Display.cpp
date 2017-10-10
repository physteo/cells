#include "Display.h"

namespace visuals {

	Display::Display(int width, int height, Box* box, PartSpecs* partSpecs, SystemTrajectory* trajectory, bool lightRendering) :
		m_window{ "Cells", width, height },
		m_box{box},
		m_camera(glm::vec3{ 0.0f,0.0f,1.0f }, glm::vec3{ 0.0f,0.0f,-1.0f }, glm::vec3{ 0.0f,1.0f,0.0f }, &m_window),
		m_renderer{ &m_camera },
		m_shader{},
		m_trajectory(trajectory),
		m_activeSite{ (GLuint) width * 1.0 * partSpecs->partTypes.at(0).sig / m_box->getLx(), (GLuint) height * 1.0 * partSpecs->partTypes.at(0).sig / m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/smile.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_inactiveSite{ (GLuint)width * 1.0 * partSpecs->partTypes.at(partSpecs->numberOfTypes - 1).sig / m_box->getLx(), (GLuint)height * 1.0 * partSpecs->partTypes.at(partSpecs->numberOfTypes - 1).sig / m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/redsiteCool.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_subBox{ (GLuint)width * 0.9 * m_box->getBoxCellLengthX() / m_box->getLx(), (GLuint)height * 0.9 * m_box->getBoxCellLengthY()/ m_box->getLy(),
		{ "C:/Users/matte/Pictures/graphics_engine/coolattice/subBox.png" }, tmx::graphics::BGRA, tmx::graphics::RGBA, 0, 0 ,10.f,0.f,true },
		m_renderablesPool{ lightRendering ? trajectory->at(0).size() : (trajectory->at(0).totalNumberOfParts() + m_box->getBoxCells().size()) },
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
		//glm::mat4 projectionMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		glm::mat4 projectionMatrix = glm::ortho((GLfloat)0.0f, (GLfloat)width, (GLfloat)0.0f, (GLfloat)height, 0.2f, 50.f);
		m_shader.loadMatrix("projectionMatrix", projectionMatrix);
		//m_shader.loadVector("lightPosition", light.getPosition());
		//m_shader.loadVector("lightColour", light.getColour());
		//m_shader.loadVector("skyColor", glm::vec3{ skyR,skyG,skyB });
		m_shader.stop();
		//int numberOfSites = m_trajectory->at(0).size();
		//m_renderables = new std::vector<tmx::graphics::Renderable>;
		//m_renderables->resize(numberOfSites);


	}

	//Display::~Display()
	//{
	//	delete m_renderables;
	//}

	void Display::run(int start, int end, int stride, int stepWait)
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
			//if (step < 2) {
				fillRenderables(step);
			//}

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
					renderable.init(&m_activeSite, &m_shader, glm::vec3{ (GLfloat)pbcPosition.x * m_window.getWidth() / m_box->getLx() ,//+   0.5 * m_window.getWidth(),
																		 (GLfloat)pbcPosition.y * m_window.getHeight() / m_box->getLy() ,//+   0.5 * m_window.getHeight(),
																		 (GLfloat) 0.0f },
						glm::vec3{ 0,1,0 },
						0.0f,
						1.0f);
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