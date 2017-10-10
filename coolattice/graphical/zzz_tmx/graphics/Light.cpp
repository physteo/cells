#include "Light.h"

namespace tmx { namespace graphics {

	void Light::setPosition(const glm::vec3& position)
	{
		m_position = position;
	}
	void Light::setColour(const glm::vec3& colour)
	{
		m_colour = colour;
	}

} }