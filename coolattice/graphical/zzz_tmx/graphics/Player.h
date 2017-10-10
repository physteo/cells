#pragma once
#include "Renderable.h"
#include "../../graphics/Window.h"

namespace tmx { namespace graphics {

	class Player : public Renderable
	{
		const float SPEED = 20;
		const float ROTSPEED = 160;

		const float GRAVITY = -98.1;
		const float JUMP_POWER = 45;
		const float TERRAIN_HEIGHT = 1;

		float m_currentSpeed;
		float m_currentTurnSpeed;
		float m_upwardSpeed;

		bool inAir() const;

		const sparky::graphics::Window* const m_window;

	public:
		Player(const sparky::graphics::Window* const window, const MTexturedModel* in_texturedModel, const ShaderProgram* in_shader,
			glm::vec3 in_position, glm::vec3 in_rotation, GLfloat in_angle, GLfloat in_scale) : 
			Renderable(in_texturedModel, in_shader,	in_position,  in_rotation,  in_angle,  in_scale), 
			m_currentSpeed(0.0f), m_currentTurnSpeed(0.0f), m_upwardSpeed(0.0f), m_window(window) {}

		void move();
		void checkInputs();

	};

} }
