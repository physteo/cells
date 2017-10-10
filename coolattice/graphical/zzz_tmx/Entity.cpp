#include "Entity.h"


namespace tmx { namespace entities {


	void Entity::increasePosition(const glm::vec3& dr) {
		m_position += dr;
	}

	void Entity::increaseRotation(const glm::vec3& dtheta) {
		m_rotation += dtheta;
	}

	void Entity::increaseAngle(GLfloat angle)
	{ 
		m_angle += angle;
	}

	void Entity::setPosition(const glm::vec3& position)
	{
		m_position = position;
	}

	void Entity::setRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
	}

	void Entity::setScale(GLfloat scale)
	{
		m_scale = scale;
	}

	void Entity::bindModel() const
	{
		m_texturedModel->bindAll();
	}
	void Entity::unbindModel() const
	{
		m_texturedModel->unbindAll();
	}
	size_t Entity::getVertexCount() const
	{
		return m_texturedModel->getVertexCount();
	}

}}