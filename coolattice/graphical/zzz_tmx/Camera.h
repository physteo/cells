#pragma once
#include "../maths/maths.h"
#include "../graphics/Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_access.hpp>

#define SPEEDUP 100

namespace tmx { namespace graphics {

	class Frustum {

	public:
		static enum { OUTSIDE = 0, INTERSECT, INSIDE };

		glm::vec3 m_camPosition; // camera position
		glm::vec3 m_X, m_Y, m_Z; // the camera referential
		float m_ratio, m_nearD, m_farD;

		float m_width, m_height, m_tang;

		Frustum();

		void setCamInternals(float angle, float ratio, float nearD, float farD);
		void setCamDef(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
		int  pointInFrustum(const glm::vec3& point);
	};

	class XCamera
	{
		glm::vec3 m_position;
		float m_pitch;
		float m_yaw;
		float m_roll;
		const sparky::graphics::Window* const m_window;

	public:
		XCamera(const sparky::graphics::Window* const window)
			: m_position(), m_pitch(0), m_yaw(0), m_roll(0), m_window(window) {}
		XCamera(const glm::vec3& position,
			float pitch, float yaw, float roll,
			const sparky::graphics::Window* const window)
			: m_position(position), m_pitch(pitch), m_yaw(yaw), m_roll(roll), m_window(window) {}

		void move();
		glm::mat4 getViewMatrix() const;

		inline const glm::vec3& getPosition() { return m_position; }
		inline float getPitch() { return m_pitch; }
		inline float getYaw()   { return m_yaw; }
		inline float getRoll()  { return m_roll; }
	};



	class Camera
	{
		glm::vec3 m_eye;
		glm::vec3 m_center;
		glm::vec3 m_up;
		const sparky::graphics::Window* const m_window;

		Frustum m_frustum;

	public:
		Camera() : m_eye(), m_center(), m_up(), m_window() {}
		Camera(const sparky::graphics::Window* const window)
			: m_eye(), m_center(0), m_up(0), m_window(window) {}
		Camera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up,
			const sparky::graphics::Window* const window)
			: m_eye(eye), m_center(center), m_up(up), m_window(window) {}

		void move();

		glm::mat4 getViewMatrix() const
		{
			return glm::lookAt(m_eye, m_center, m_up);
		}

		void printCamData() const
		{
			std::cout << "Eye: ( " << m_eye.x << ", " << m_eye.y << ", " << m_eye.z << " )" << std::endl;
			std::cout << "Cen: ( " << m_center.x << ", " << m_center.y << ", " << m_center.z << " )" << std::endl;
			std::cout << "Upv: ( " << m_up.x << ", " << m_up.y << ", " << m_up.z << " )" << std::endl;
			std::cout << std::endl;
		}

		inline Frustum& getFrustum() { return m_frustum; }
		inline const Frustum& getFrustum() const { return m_frustum; }


		inline const glm::vec3& getPosition() const { return m_eye; }
		inline       glm::vec3& getPosition() { return m_eye; }

		inline const glm::vec3& getCenter() { return m_center; }
	private:
		void updateFrustum();
	};



} }
