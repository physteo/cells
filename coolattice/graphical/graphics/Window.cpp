#include "window.h"

namespace sparky { namespace graphics {




    void window_resize(GLFWwindow* window, int width, int height);


    Window::Window(const char* name, int width, int height)
        : m_Name(name), m_Width(width), m_Height(height), m_lastFrameTime(glfwGetTime()), m_delta(0.0f)
    {

        if(!init())
            glfwTerminate();


        for (int i = 0; i < MAX_KEYS; i++)
        {
            m_Keys[i] = false;
        }

        for (int i = 0; i < MAX_BUTTONS; i++)
        {
            m_MouseButtons[i] = false;
        }

    }


    Window::~Window()
    {
        glfwTerminate();
    }


    bool Window::init()
    {

        if(!glfwInit()) {
            std::cout << "Failure GLFW" << std::endl;
            return false;
        }


		// those three lines avoid segfault in linux, but it draws nothing.
		// On windows, drawing works only by disabling them
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Name, NULL, NULL);
        if(!m_Window)
        {
            std::cout << "Failed to create window" << std::endl;
            return false;
        }

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, this);
        glfwSetWindowSizeCallback(m_Window, window_resize);
        glfwSetKeyCallback(m_Window, key_callback);
        glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
        glfwSetCursorPosCallback(m_Window, cursor_position_callback);


        // after the creation of the context (or glew will not initialize!!)
        glewExperimental = GL_TRUE;
        if(glewInit() != GLEW_OK)
        {
            std::cout << "Could not initialize GLEW!" << std::endl;
            return false;
        }
        std::cout << glGetString(GL_VERSION) << std::endl;
        std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        return true;
    }


	void Window::makeCurrentContext() const
	{
		glfwMakeContextCurrent(m_Window);
	}


    bool Window::isKeyPressed(unsigned int keycode) const
    {
        // TODO: Log this!
        if(keycode >= MAX_KEYS)
            return false;

        return m_Keys[keycode];
    }

    bool Window::isMouseButtonPressed(unsigned int button) const
    {
        // TODO: Log this!
        if(button >= MAX_BUTTONS)
            return false;

        return m_MouseButtons[button];
    }


    void Window::getMousePosition(double& x, double& y) const
    {
        x = mx;
        y = my;
    }


    void Window::clear() const
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }



    void Window::update()
    {
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL error: " << error << std::endl;
		}

        glfwPollEvents();

        glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
        glfwSwapBuffers(m_Window);

		double currentFrameTime = glfwGetTime();
		m_delta = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;
    }



    bool Window::closed() const
    {
        return glfwWindowShouldClose(m_Window) == 1;
    }


    void window_resize(GLFWwindow* window, int width, int height)
    {
        // TODO: change also width and height
        glViewport(0, 0, width, height);

    }


    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        // this way we can access the window itself
        Window* win = (Window*) glfwGetWindowUserPointer(window);
        // if and only if the key is released, the key is set to false
        win->m_Keys[key] = action != GLFW_RELEASE;
    }

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        Window* win = (Window*) glfwGetWindowUserPointer(window);
        win->m_MouseButtons[button] = action != GLFW_RELEASE;
    }


    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Window* win = (Window*) glfwGetWindowUserPointer(window);
        win->mx = xpos;
        win->my = ypos;
    }

}

}
