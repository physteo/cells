#pragma once

#include <deque>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "../Camera.h"

#include "../ObjectPool.h"
#include "../physics/Particle.h"

namespace tmx {
	namespace graphics {

		class InstanceRenderer : public Renderer
		{
			std::deque<const Renderable*> m_renderables;

			const Camera* m_camera;

		public:
			InstanceRenderer(const Camera* camera) : m_camera(camera) {}

			void enableCulling() const
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}

			void disableCulling() const
			{
				glDisable(GL_CULL_FACE);
			}

			void submit(const Renderable* renderable) override {
				m_renderables.push_front(renderable);
			}

			

			void flush() override {

				if (!m_renderables.empty())
				{
					const Renderable* m_renderable = m_renderables.front();
				
					
					if (m_renderable->texturedModel->hasTransparency())
					{
						disableCulling();
					}


					// activate shader
					m_renderable->shader->start();
					// pass the uniforms to the shader
					auto viewMatrix = m_camera->getViewMatrix();
					m_renderable->shader->loadMatrix("viewMatrix", viewMatrix);
					// transformation matrix? will be calculated inside the shader unsing the position of the instance given as an attribute
					//m_renderable->shader->loadFloat("shineDamper", m_renderable->texturedModel->getShineDamper());
					//m_renderable->shader->loadFloat("reflectivity", m_renderable->texturedModel->getReflectivity());
					for (size_t i = 0; i < m_renderable->texturedModel->getNumberOfTextures(); i++)
					{
						std::string name = "texture";
						name += std::to_string(i);
						//std::cout << name << std::endl;
						m_renderable->shader->loadInt(name.c_str(), i);
					}


					// activate VAO and texture
					m_renderable->texturedModel->bindAll();

					// prepare array of offsets
					std::vector<GLfloat> offsets;
					//o/offsets.reserve(3 * m_renderables.size());
					offsets.reserve(3 * m_renderables.size());
					for (size_t n = 0; n < m_renderables.size(); n++)
					{
						offsets.push_back(m_renderables.at(n)->position.x);
						offsets.push_back(m_renderables.at(n)->position.y);
						offsets.push_back(m_renderables.at(n)->position.z);
					}

					// add new VBO to the VAO
					GLuint instanceVBO;
					glGenBuffers(1, &instanceVBO);
					glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_renderables.size(), &offsets[0], GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);


					glEnableVertexAttribArray(3);
					glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
					glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glVertexAttribDivisor(3, 1); // Tell OpenGL this is an instanced vertex attribute.



					glDrawElementsInstanced(GL_TRIANGLES, m_renderable->texturedModel->getIndicesCount(), GL_UNSIGNED_INT, nullptr, m_renderables.size());


					glDeleteBuffers(1, &instanceVBO);

					m_renderable->texturedModel->unbindAll();
					m_renderable->shader->stop();

					if (m_renderable->texturedModel->hasTransparency())
					{
						enableCulling();
					}

					
					m_renderables.clear();
				}
				




			}

		};


		class XInstanceRenderer : public Renderer
		{
			std::vector<std::deque<const Renderable*> > m_tableOfRenderables;
			std::vector<std::pair<const MTexturedModel*, const ShaderProgram*> > models;

			const Camera* m_camera;

		public:
			XInstanceRenderer(const Camera* camera) : m_camera(camera) {}

			void enableCulling() const
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK); // enabling this will not show quads from behind!
			}

			void disableCulling() const
			{
				glDisable(GL_CULL_FACE);
			}

			void submit(const Renderable* renderable) override {

				const MTexturedModel* model = renderable->texturedModel;
				const ShaderProgram*  shader = renderable->shader;

				bool found = false;
				for (size_t i = 0; i < models.size(); i++)
				{
					if (model == models.at(i).first && shader == models.at(i).second )
					{
						m_tableOfRenderables.at(i).push_front(renderable);
						found = true;
					}

				}
				if (!found)
				{
					std::deque<const Renderable*> newDeque;
					newDeque.push_front(renderable);
					m_tableOfRenderables.push_back(newDeque);

					models.push_back({ model, shader });
				}
			}



			void flush() override {

				for(size_t d = 0; d < m_tableOfRenderables.size(); d++)
				{
					std::deque<const graphics::Renderable*>& m_renderables = m_tableOfRenderables.at(d);

					if (!m_renderables.empty())
					{
						const MTexturedModel* texturedModel = models.at(d).first;
						const ShaderProgram* shader = models.at(d).second;

						if (texturedModel->hasTransparency())
						{
							disableCulling();
						}


						// activate shader
						shader->start();
						// pass the uniforms to the shader
						auto viewMatrix = m_camera->getViewMatrix();
						shader->loadMatrix("viewMatrix", viewMatrix);
						// transformation matrix? will be calculated inside the shader unsing the position of the instance given as an attribute
						shader->loadFloat("shineDamper", texturedModel->getShineDamper());
						shader->loadFloat("reflectivity", texturedModel->getReflectivity());
						for (size_t i = 0; i < texturedModel->getNumberOfTextures(); i++)
						{
							std::string name = "texture";
							name += std::to_string(i);
							//std::cout << name << std::endl;
							shader->loadInt(name.c_str(), i);
						}


						// activate VAO and texture
						texturedModel->bindAll();

						// prepare array of offsets
						std::vector<GLfloat> offsets;
						// ciaooooooooooooo

						//o/offsets.reserve(3 * m_renderables.size());
						offsets.reserve(3 * m_renderables.size());

						for (size_t n = 0; n < m_renderables.size(); n++)
						{
							offsets.push_back(m_renderables.at(n)->position.x);
							offsets.push_back(m_renderables.at(n)->position.y);
							offsets.push_back(m_renderables.at(n)->position.z);
						}

						std::array<std::vector<glm::vec4>, 4> rotationMatricesColumns;
						rotationMatricesColumns[0].reserve(m_renderables.size());
						rotationMatricesColumns[1].reserve(m_renderables.size());
						rotationMatricesColumns[2].reserve(m_renderables.size());
						rotationMatricesColumns[3].reserve(m_renderables.size());

						for (size_t n = 0; n < m_renderables.size(); n++)
						{
							auto rotation = glm::rotate(glm::mat4(1.0), m_renderables.at(n)->angle, m_renderables.at(n)->rotation);
							rotationMatricesColumns[0].push_back(glm::column(rotation, 0));
							rotationMatricesColumns[1].push_back(glm::column(rotation, 1));
							rotationMatricesColumns[2].push_back(glm::column(rotation, 2));
							rotationMatricesColumns[3].push_back(glm::column(rotation, 3));
						}



						// checkpoint2

						// add new VBO to the VAO
						GLuint instanceVBO;
						glGenBuffers(1, &instanceVBO);
						glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
						glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_renderables.size(), &offsets[0], GL_STATIC_DRAW);
						glBindBuffer(GL_ARRAY_BUFFER, 0);


						glEnableVertexAttribArray(3);
						glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
						glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glVertexAttribDivisor(3, 1); // Tell OpenGL this is an instanced vertex attribute.


						// load rotation matrix
						GLuint matrixVBOs[4];
						for (size_t column = 0; column < 4; column++)
						{
							GLfloat* pointerToData = &rotationMatricesColumns[column][0][0];
							glGenBuffers(1, &matrixVBOs[column]);
							glBindBuffer(GL_ARRAY_BUFFER, matrixVBOs[column]);
							glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * m_renderables.size(), pointerToData, GL_STATIC_DRAW);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
						
						
							glEnableVertexAttribArray(4 + column);
							glBindBuffer(GL_ARRAY_BUFFER, matrixVBOs[column]);
							glVertexAttribPointer(4 + column, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glVertexAttribDivisor(4 + column, 1); // Tell OpenGL this is an instanced vertex attribute.
						}

						

						glDrawElementsInstanced(GL_TRIANGLES, texturedModel->getIndicesCount(), GL_UNSIGNED_INT, nullptr, m_renderables.size());

						
						glDeleteBuffers(1, &instanceVBO);
						glDeleteBuffers(1, &matrixVBOs[0]);
						glDeleteBuffers(1, &matrixVBOs[1]);
						glDeleteBuffers(1, &matrixVBOs[2]);
						glDeleteBuffers(1, &matrixVBOs[3]);


						texturedModel->unbindAll();
						shader->stop();

						if (texturedModel->hasTransparency())
						{
							enableCulling();
						}


						m_renderables.clear();
					}
				}


			}

		};



/*
		class MInstanceRenderer : public Renderer
		{
			//o/std::deque<const Renderable*> m_renderables;
			const Renderable* m_renderable;
			size_t m_numberOfInstances;

			//const glm::vec3* m_positions;
			const Camera* m_camera;

			const memory::ObjectPool<physics::Particle>* m_data;

		public:
			MInstanceRenderer(const Camera* camera) : m_camera(camera) {}

			void enableCulling() const
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}

			void disableCulling() const
			{
				glDisable(GL_CULL_FACE);
			}

			void submit(const Renderable* renderable) override {
				//o/m_renderables.push_front(renderable);
				m_renderable = renderable;
			}

			void getInstancedData(const memory::ObjectPool<physics::Particle>* data)
			{
				m_numberOfInstances = data->getActiveElements();
				m_data = data;
			}



			void flush() override {

				//o/if (!m_renderables.empty())
				//o/{
				//o/const Renderable* renderable = m_renderables.front();
				if (m_data) {
					//o/if (renderable->texturedModel->hasTransparency())
					if (m_renderable->texturedModel->hasTransparency())
					{
						disableCulling();
					}


					// activate shader
					m_renderable->shader->start();
					// pass the uniforms to the shader
					auto viewMatrix = m_camera->getViewMatrix();
					m_renderable->shader->loadMatrix("viewMatrix", viewMatrix);
					// transformation matrix? will be calculated inside the shader unsing the position of the instance given as an attribute
					m_renderable->shader->loadFloat("shineDamper", m_renderable->texturedModel->getShineDamper());
					m_renderable->shader->loadFloat("reflectivity", m_renderable->texturedModel->getReflectivity());
					for (size_t i = 0; i < m_renderable->texturedModel->getNumberOfTextures(); i++)
					{
						std::string name = "texture";
						name += std::to_string(i);
						//std::cout << name << std::endl;
						m_renderable->shader->loadInt(name.c_str(), i);
					}


					// activate VAO and texture
					m_renderable->texturedModel->bindAll();

					// prepare array of offsets
					std::vector<GLfloat> offsets;
					//o/offsets.reserve(3 * m_renderables.size());
					offsets.reserve(3 * m_numberOfInstances);
					for (size_t n = 0; n < m_numberOfInstances; n++)
					{
						offsets.push_back(m_data->at(n).getPosition().x);
						offsets.push_back(m_data->at(n).getPosition().y);
						offsets.push_back(m_data->at(n).getPosition().z);
					}

					// add new VBO to the VAO
					GLuint instanceVBO;
					glGenBuffers(1, &instanceVBO);
					glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_numberOfInstances, &offsets[0], GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);


					glEnableVertexAttribArray(3);
					glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
					glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glVertexAttribDivisor(3, 1); // Tell OpenGL this is an instanced vertex attribute.



					glDrawElementsInstanced(GL_TRIANGLES, m_renderable->texturedModel->getIndicesCount(), GL_UNSIGNED_INT, nullptr, m_numberOfInstances);


					glDeleteBuffers(1, &instanceVBO);

					m_renderable->texturedModel->unbindAll();
					m_renderable->shader->stop();

					if (m_renderable->texturedModel->hasTransparency())
					{
						enableCulling();
					}

					m_data = nullptr;
					//o/m_renderables.clear();
					//o/}
				}




			}

		};





*/


	}
}

