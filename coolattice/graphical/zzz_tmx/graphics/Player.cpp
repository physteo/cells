#include "Player.h"

namespace tmx { namespace graphics {


	void Player::move()
	{
		checkInputs();

		float deltat = m_window->getFrameTimeSeconds();

		Renderable::angle += m_currentTurnSpeed * deltat;
		float distance = m_currentSpeed * deltat;
		float dx = (float) distance * glm::sin(glm::radians(Renderable::angle));
		float dz = (float) distance * glm::cos(glm::radians(Renderable::angle));
		Renderable::position += glm::vec3{ dx, 0, dz };

		m_upwardSpeed +=  GRAVITY * deltat;
		Renderable::position.y += m_upwardSpeed * deltat;
		if (Renderable::position.y < TERRAIN_HEIGHT) {
			m_upwardSpeed = 0;
			Renderable::position.y = TERRAIN_HEIGHT;
		}
	}

	bool Player::inAir() const
	{
		if (Renderable::position.y <= TERRAIN_HEIGHT) {
			return false;
		}
		else {
			return true;
		}
	}

	void Player::checkInputs()
	{
		if (m_window->isKeyPressed(GLFW_KEY_I)){
			m_currentSpeed = SPEED;
		}
		else if (m_window->isKeyPressed(GLFW_KEY_K)) {
			m_currentSpeed = -SPEED;
		}
		else {
			m_currentSpeed = 0.0f;
		}


		if (m_window->isKeyPressed(GLFW_KEY_L)) {
			m_currentTurnSpeed = -ROTSPEED;
		}
		else if (m_window->isKeyPressed(GLFW_KEY_J)) {
			m_currentTurnSpeed = ROTSPEED;
		}
		else {
			m_currentTurnSpeed = 0.0f;
		}

		if (m_window->isKeyPressed(GLFW_KEY_SPACE) && !inAir()) {
			m_upwardSpeed = JUMP_POWER;
		}


	}

} }