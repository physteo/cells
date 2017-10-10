#include "Camera.h"

namespace tmx { namespace graphics {

	Frustum::Frustum()
	{

	}

	void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD)
	{
		// store the information
		m_ratio = ratio;
		m_nearD = nearD;
		m_farD = farD;

		// compute width and height of the near section
		m_tang = (float)glm::tan( glm::radians(angle * 0.5));
		m_height = nearD * m_tang;
		m_width = m_height * ratio;
	}

	void Frustum::setCamDef(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	{
		m_camPosition = eye;

		// compute the Z axis of the camera referential
		// this axis points in the same direction from
		// the looking direction
		m_Z = center - eye;
		m_Z = glm::normalize(m_Z);

		// X axis of camera is the cross product of Z axis and given "up" vector 
		m_X = glm::cross(m_Z, up);
		m_X = glm::normalize(m_X);

		// the real "up" vector is the cross product of X and Z
		m_Y = glm::cross(m_X, m_Z);
	}

	int  Frustum::pointInFrustum(const glm::vec3& point)
	{

		float pcz, pcx, pcy, aux;

		// compute vector from camera position to p
		glm::vec3 pFromEye = point - m_camPosition;

		// compute and test the Z coordinate
		pcz = glm::dot(pFromEye, m_Z);

		if (pcz > m_farD || pcz < m_nearD) {
			return(OUTSIDE);
		}

		// compute and test the Y coordinate
		pcy = glm::dot(pFromEye, m_Y);
		aux = pcz * m_tang;
		if (pcy > aux || pcy < -aux)
			return(OUTSIDE);

		// compute and test the X coordinate

		pcx = glm::dot(pFromEye, m_X);
		aux = aux * m_ratio;
		if (pcx > aux || pcx < -aux)
			return(OUTSIDE);

		return(INSIDE);
	}


	void XCamera::move()
	{
		if (m_window->isKeyPressed(GLFW_KEY_S))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 addedPosition = glm::rotate(glm::vec3{ 0.f,0.f,delta }, -m_pitch, glm::vec3{ 1.0f,0.0f,0.0f });
			addedPosition = glm::rotate(addedPosition,-m_yaw, glm::vec3{ 0.0f,1.0f,0.0f });
			m_position += addedPosition;
		}
		if (m_window->isKeyPressed(GLFW_KEY_A))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 addedPosition = glm::rotate(glm::vec3{ -delta,0.f,0.f }, -m_yaw, glm::vec3{ 0.0f,1.0f,0.0f });
			m_position += addedPosition;

		}
		if (m_window->isKeyPressed(GLFW_KEY_D))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 addedPosition = glm::rotate(glm::vec3{ delta,0.f,0.f }, -m_yaw, glm::vec3{ 0.0f,1.0f,0.0f });
			m_position += addedPosition;

		}
		if (m_window->isKeyPressed(GLFW_KEY_W))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 addedPosition = glm::rotate(glm::vec3{ 0.f,0.f,-delta }, -m_pitch, glm::vec3{ 1.0f,0.0f,0.0f });
			addedPosition = glm::rotate(addedPosition, -m_yaw, glm::vec3{ 0.0f,1.0f,0.0f });
			m_position += addedPosition;
		}
		if (m_window->isKeyPressed(GLFW_KEY_Q))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_yaw -= delta;
		}
		if (m_window->isKeyPressed(GLFW_KEY_E))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_yaw += delta;
		}
		if (m_window->isKeyPressed(GLFW_KEY_R))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_pitch -= delta;
		}
		if (m_window->isKeyPressed(GLFW_KEY_F))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_pitch += delta;
		}
		if (m_window->isKeyPressed(GLFW_KEY_X))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 addedPosition = glm::rotate(glm::vec3{ 0.f,+delta,0.0f }, -m_yaw, glm::vec3{ 0.0f,1.0f,0.0f });
			
			m_position += addedPosition;
		}
		if (m_window->isKeyPressed(GLFW_KEY_Z))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 addedPosition = glm::rotate(glm::vec3{ 0.f,-delta,0.0f }, -m_yaw, glm::vec3{ 0.0f,1.0f,0.0f });
			
			m_position += addedPosition;
		}
	}


	glm::mat4 XCamera::getViewMatrix() const
	{
		//TODO: ROLL NOT IMPLEMENTED

		// Set identity
		glm::mat4 result(1.0f);
		result = glm::rotate(result, m_pitch, glm::vec3{ 1.0f,0.0f,0.0f });
		result = glm::rotate(result, m_yaw, glm::vec3{ 0.0f,1.0f,0.0f });
		result = glm::translate(result, -m_position);
		
		return result;
	}


	



	void Camera::move()
	{
		bool moved = false;
		if (m_window->isKeyPressed(GLFW_KEY_S))
		{
			GLfloat delta = 0.2f;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }	
			glm::vec3 addedPos = glm::normalize(m_eye - m_center) * delta;
			m_eye += addedPos;
			m_center += addedPos;
			moved = true;
		}
		
		if (m_window->isKeyPressed(GLFW_KEY_A))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 axis = glm::normalize(glm::cross(m_up, m_center - m_eye));
			m_eye += axis * delta;
			m_center += axis * delta;
			moved = true;
		}
		if (m_window->isKeyPressed(GLFW_KEY_D))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 axis = glm::normalize(glm::cross(m_up, m_center - m_eye));
			m_eye -= axis * delta;
			m_center -= axis * delta;
			moved = true;
		}
		
		if (m_window->isKeyPressed(GLFW_KEY_W))
		{
			GLfloat delta = 0.2f;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 addedPos = glm::normalize(m_eye - m_center) * delta;
			m_eye -= addedPos;
			m_center -= addedPos;
			moved = true;
		}
		
		if (m_window->isKeyPressed(GLFW_KEY_Q))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_center = m_eye + glm::rotate(m_center - m_eye, delta, m_up);
			moved = true;
		}
		if (m_window->isKeyPressed(GLFW_KEY_E))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_center = m_eye + glm::rotate(m_center - m_eye, -delta, m_up);
			moved = true;
		}
		if (m_window->isKeyPressed(GLFW_KEY_R))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 axis = glm::normalize(glm::cross(m_up, m_center - m_eye));
			//m_up = glm::rotate(m_up, -delta, axis);
			//axis = glm::normalize(glm::cross(m_up, m_center - m_eye));
			glm::vec3 newCenter_minus_eye = glm::rotate(m_center - m_eye, -delta, axis);
			if (glm::dot(glm::normalize(newCenter_minus_eye), glm::normalize(m_up)) < 0.95)
				m_center = newCenter_minus_eye + m_eye;
			moved = true;
		}
		if (m_window->isKeyPressed(GLFW_KEY_F))
		{
			GLfloat delta = 2.f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			glm::vec3 axis = glm::normalize(glm::cross(m_up, m_center - m_eye));
			//m_up = glm::rotate(m_up, -delta, axis);
			//axis = glm::normalize(glm::cross(m_up, m_center - m_eye));
			glm::vec3 newCenter_minus_eye = glm::rotate(m_center - m_eye, delta, axis);
			if (glm::dot(glm::normalize(newCenter_minus_eye), glm::normalize(m_up)) > -0.95)
				m_center = newCenter_minus_eye + m_eye;
			moved = true;
		}
		if (m_window->isKeyPressed(GLFW_KEY_X))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_eye.y += delta;
			m_center.y += delta;
			moved = true;
		}
		if (m_window->isKeyPressed(GLFW_KEY_Z))
		{
			GLfloat delta = 0.2f;;
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) { delta *= SPEEDUP; }
			if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) { delta /= SPEEDUP; }
			m_eye.y -= delta;
			m_center.y -= delta;
			moved = true;
		}

		if (moved)
			updateFrustum();
	}


	void Camera::updateFrustum()
	{
		m_frustum.setCamDef(m_eye, m_center, m_up);
	}


}}