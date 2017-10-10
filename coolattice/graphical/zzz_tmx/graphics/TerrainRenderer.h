#pragma once

#include <deque>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "../Camera.h"

namespace tmx {
	namespace graphics {

		//! Right now it is exactly like SimpleRenderer. It will need to be different later.
		class TerrainRenderer : public Renderer
		{
			std::deque<const Renderable*> m_renderables;
			const Camera* m_camera;
		public:
			TerrainRenderer(const Camera* camera) : m_camera(camera) {}

			void submit(const Renderable* renderable) override {
				m_renderables.push_front(renderable);
			}
			void flush() override {
				while (!m_renderables.empty())
				{
					const Renderable* renderable = m_renderables.front();

					renderable->shader->start();
					renderable->texturedModel->bindAll();

					auto viewMatrix = m_camera->getViewMatrix();
					auto translation = glm::translate(glm::mat4(1.0), renderable->position);
					auto rotation = glm::rotate(glm::mat4(1.0), renderable->angle, renderable->rotation);
					// first traslation and then rotation... that's because of opengl's fucked up column major order
					auto transformationMatrix = translation * rotation;

					renderable->shader->loadMatrix("viewMatrix", viewMatrix);
					renderable->shader->loadMatrix("transformationMatrix", transformationMatrix);
					//// TODO: put this stuff inside the shader, so that it is different according to the shader we use
					renderable->shader->loadFloat("shineDamper", renderable->texturedModel->getShineDamper());
					renderable->shader->loadFloat("reflectivity", renderable->texturedModel->getReflectivity());
					// End TODO

					glDrawElements(GL_TRIANGLES, renderable->texturedModel->getIndicesCount(), GL_UNSIGNED_INT, nullptr);

					renderable->texturedModel->unbindAll();
					renderable->shader->stop();

					m_renderables.pop_front();
				}
			}

		};



	}
}