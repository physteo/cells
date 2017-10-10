#pragma once

#include <MYGLEW/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

namespace sparky {
	namespace graphics {

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

		class Window;
		//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		//static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		//static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

		class Window
		{

		private:
			const char* m_Name;
			int m_Width, m_Height;
			GLFWwindow* m_Window;
			bool m_Closed;

			bool m_Keys[MAX_KEYS];
			bool m_MouseButtons[MAX_BUTTONS];
			double mx, my;

			double m_lastFrameTime;
			float  m_delta;

		public:
			Window() : m_Name("DefaultName"), m_Width(640), m_Height(480) {}
			Window(const char* name, int width, int height);
			~Window();
			void clear() const;
			bool closed() const;
			void update();

			inline int getWidth() const { return m_Width; }
			inline int getHeight() const { return m_Height; }

			bool isKeyPressed(unsigned int keycode) const;
			bool isMouseButtonPressed(unsigned int button) const;
			void getMousePosition(double& x, double& y) const;

			inline float getFrameTimeSeconds() const { return m_delta; }
		private:
			bool init();
			inline double getCurrentTime() const { return glfwGetTime(); }
			// we make it friend so that we can modify the members of the window through key_callback
			friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		};





	}

}

