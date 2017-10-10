#pragma once

#include <iostream>
#include <vector>
#include <MYGLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "XTexturedModel.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
// TODO: put this outside
#include "ShaderProgram.h"

typedef std::vector< std::pair<tmx::graphics::MyTexturedModel, std::vector<tmx::entities::Entity> > > EntitiesRegistry;

namespace tmx { namespace renderEngine {

	class MasterRenderer
	{
	public:
		MasterRenderer() {}

		void render(const EntitiesRegistry& registry, const tmx::graphics::Light& light, 
			const tmx::graphics::Camera& camera, const tmx::graphics::StaticShader& shader)
		{
			// go through the registry
			auto unitMatrix = glm::mat4(1.0f);
			
			for (size_t model = 0; model < registry.size(); model++) {
				const tmx::graphics::MyTexturedModel& texturedModel = registry.at(model).first;
				const std::vector<tmx::entities::Entity>& entities = registry.at(model).second;

				// bind texturedModel
				texturedModel.bindAll();
				// render all entities sharing the same texturedModel
				for (size_t e = 0; e < entities.size(); e++)
				{
					const tmx::entities::Entity& entity = entities.at(e);
					auto traslation = glm::translate(unitMatrix, entity.getPosition());
					auto rotation = glm::rotate(unitMatrix, entity.getAngle(), entity.getRotation());
					// first traslation and then rotation... that's because of opengl's fucked up column major order
					auto transformationMatrix = traslation * rotation;

					shader.loadMatrix("transformationMatrix", transformationMatrix);
					// TODO: put this stuff inside the shader, so that it is different according to the shader we use
					shader.loadFloat("shineDamper", texturedModel.getShineDamper());
					shader.loadFloat("reflectivity", texturedModel.getReflectivity());
					// End TODO

					glDrawElements(GL_TRIANGLES, texturedModel.getVertexCount(), GL_UNSIGNED_INT, nullptr);

				}

				// unbind texturedModel
				texturedModel.unbindAll();
			}
		}


	};


} }
