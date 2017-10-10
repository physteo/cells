#pragma once

#include <glm/glm.hpp>

namespace tmx { namespace graphics {

	class Light
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_colour;
	public:
		Light(const glm::vec3& position, const glm::vec3& colour) :
			m_position(position), m_colour(colour) {}

		inline glm::vec3 getPosition() const { return m_position; }
		inline glm::vec3 getColour() const { return m_colour; }

		void setPosition(const glm::vec3& position);
		void setColour(const glm::vec3& colour);

	};


} }



