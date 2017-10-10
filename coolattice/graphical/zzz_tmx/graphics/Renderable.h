#pragma once

#include <MYGLEW/glew.h>
#include <glm/glm.hpp>
#include "../ShaderProgram.h"
#include "MTexturedModel.h"

#include "../gameObjects/ObjectWithID.h"

namespace tmx { namespace graphics {

	class Renderable : public gameObjects::SelectableObjectWithID
	{
	public:
		const MTexturedModel* texturedModel;
		const ShaderProgram* shader;

		glm::vec3 position;
		glm::vec3 rotation;
		GLfloat angle;
		GLfloat scale;

		Renderable() : SelectableObjectWithID(), texturedModel(nullptr), shader(nullptr), position(glm::vec3{0,0,0}),
			rotation(glm::vec3{ 0,1,0 }), angle(0), scale(1.0) {}

		Renderable(const MTexturedModel* in_texturedModel, const ShaderProgram* in_shader)
			: SelectableObjectWithID(false, 0), texturedModel(in_texturedModel), shader(in_shader), position(glm::vec3{ 0,0,0 }),
			rotation(glm::vec3{ 0,1,0 }), angle(0), scale(1.0) {}

		Renderable(const MTexturedModel* in_texturedModel, const ShaderProgram* in_shader,
			glm::vec3 in_position, glm::vec3 in_rotation, GLfloat in_angle, GLfloat in_scale) : 
			SelectableObjectWithID(false, 0), texturedModel(in_texturedModel), shader(in_shader), position(in_position),
			rotation(in_rotation), angle(in_angle), scale(in_scale) {}

		virtual void init(const MTexturedModel* in_texturedModel, const ShaderProgram* in_shader,
			glm::vec3 in_position, glm::vec3 in_rotation, GLfloat in_angle, GLfloat in_scale)
		{
			texturedModel = in_texturedModel;
			shader = in_shader;
			position = in_position;
			rotation = in_rotation;
			angle = in_angle;
			scale = in_scale;
		}

	};


	class GrassRenderable : public Renderable
	{
	public:
		const std::vector<const MTexturedModel*>* LODtexturedModels;

		GrassRenderable() : Renderable(), LODtexturedModels() {}

		GrassRenderable(const MTexturedModel* in_texturedModel, const std::vector<const MTexturedModel*>* in_LODtexturedModels, const ShaderProgram* in_shader) 
			: Renderable(in_texturedModel, shader), LODtexturedModels(in_LODtexturedModels) {}

		GrassRenderable(const MTexturedModel* in_texturedModel, const std::vector<const MTexturedModel*>* in_LODtexturedModels, const ShaderProgram* in_shader,
			glm::vec3 in_position, glm::vec3 in_rotation, GLfloat in_angle, GLfloat in_scale) 
			: Renderable(in_texturedModel, in_shader, in_position, in_rotation, in_angle, in_scale), LODtexturedModels(in_LODtexturedModels) {}


		void init(const MTexturedModel* in_texturedModel, const std::vector<const MTexturedModel*>* in_LODtexturedModels, const ShaderProgram* in_shader,
			glm::vec3 in_position, glm::vec3 in_rotation, GLfloat in_angle, GLfloat in_scale)
		{
			texturedModel = in_texturedModel;
			LODtexturedModels = in_LODtexturedModels;
			shader = in_shader;
			position = in_position;
			rotation = in_rotation;
			angle = in_angle;
			scale = in_scale;
		}

	};

} }