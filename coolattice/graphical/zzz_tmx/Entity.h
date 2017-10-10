#pragma once

#include "XTexturedModel.h"
#include "../maths/maths.h"
#include <glm/glm.hpp>

namespace tmx { namespace entities {


	class Entity
	{
	private:
		const graphics::MyTexturedModel* const m_texturedModel;
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		GLfloat m_angle;
		GLfloat m_scale;

	public:
		Entity(const graphics::MyTexturedModel* const texturedModel, const glm::vec3& position,
			const glm::vec3& rotation, GLfloat angle, GLfloat scale) :
			m_texturedModel(texturedModel), m_position(position), m_rotation(rotation),
			m_angle(angle), m_scale(scale) {}

		// Can be copied
		//Entity(const Entity&) = delete;
		//Entity& operator=(const Entity&) = delete;

		// Can be moved
		//Vbo(Vbo&& other);
		//Vbo& operator=(Vbo&& other);

		void increasePosition(const glm::vec3& dr);
		void increaseRotation(const glm::vec3& dtheta);
		void increaseAngle(GLfloat angle);

		inline const graphics::MyTexturedModel* const getTexturedModel() const { return m_texturedModel; }
		inline const glm::vec3& getPosition() const { return m_position; }
		inline const glm::vec3& getRotation() const { return m_rotation; }
		inline GLfloat getAngle() const { return m_angle; }
		inline GLfloat getScale() const { return m_scale; }

		void setPosition(const glm::vec3&);
		void setRotation(const glm::vec3&);
		void setScale(GLfloat);
		void setAngle(GLfloat angle) { m_angle = angle; }

		void bindModel() const;
		void unbindModel() const;
		size_t getVertexCount() const;

	private:


	};


//	class Entity
//	{
//	private:
//		const graphics::XTexturedModel* const m_model;
//		glm::vec3 m_position;
//		glm::vec3 m_rotation;
//		GLfloat m_angle;
//		//GLfloat m_rotX, m_rotY, m_rotZ;
//		GLfloat m_scale;
//
//	public:
//		Entity(const graphics::XTexturedModel* const model, const glm::vec3& position,
//			const glm::vec3& rotation, GLfloat angle, GLfloat scale) :
//			m_model(model), m_position(position), m_rotation(rotation),
//			m_angle(angle), m_scale(scale) {}
//
//		void increasePosition(const glm::vec3& dr);
//		void increaseRotation(const glm::vec3& dtheta);
//		void increaseAngle(GLfloat angle);
//
//		//inline const graphics::XTexturedModel* const getXTexturedModel() const { return m_model; }
//		inline const glm::vec3& getPosition() const { return m_position; }
//		inline const glm::vec3& getRotation() const { return m_rotation; }
//		inline GLfloat getAngle() const { return m_angle; }
//		inline GLfloat getScale() const { return m_scale; }
//
//		void setPosition(const glm::vec3&);
//		void setRotation(const glm::vec3&);
//		void setScale(GLfloat);
//		void setAngle(GLfloat angle) { m_angle = angle; }
//
//		void bindModel() const;
//		void unbindModel() const;
//		size_t getVertexCount() const;
//
//	private:
//
//
//	};


}}