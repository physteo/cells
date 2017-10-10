#include "renderEngine.h"

namespace tmx { namespace renderEngine {






	// Renderer
	void Renderer::prepare()
	{
		
	}



	void Renderer::renderVao(const tmx::graphics::Vao& vao)
	{
		vao.bindAll();
		//glDrawArrays(GL_TRIANGLES, 0, m_vbos[0].getLength() / m_vbos[0].getComponents());
		glDrawElements(GL_TRIANGLES, vao.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
		vao.unbindAll();
	}

	void Renderer::RenderXTexturedModel(const tmx::graphics::XTexturedModel& xTexturedModel)
	{
		xTexturedModel.bindAll();
		glDrawElements(GL_TRIANGLES, xTexturedModel.vao->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
		xTexturedModel.unbindAll();
	}

	void Renderer::RenderEntity(const tmx::entities::Entity& entity, tmx::graphics::StaticShader& shader)
	{
		entity.bindModel();
		auto traslation = glm::translate(glm::mat4(1.0f), entity.getPosition());
		auto rotation = glm::rotate(glm::mat4(1.0f), entity.getAngle(), entity.getRotation());
		//auto traslation = sparky::maths::mat4::translation(entity.getPosition());
		//auto rotation = sparky::maths::mat4::rotation(entity.getAngle(), entity.getRotation());
		// first traslation and then rotation... that's because of opengl's fucked up column major order
		auto transformationMatrix =  traslation * rotation;
		
		shader.loadMatrix("transformationMatrix", transformationMatrix);

		// TODO: put this stuff inside the shader, so that it is different according to the shader we use
		shader.loadFloat("shineDamper", entity.getTexturedModel()->getShineDamper());
		shader.loadFloat("reflectivity", entity.getTexturedModel()->getReflectivity());
		// End TODO

		glDrawElements(GL_TRIANGLES, entity.getVertexCount(), GL_UNSIGNED_INT, nullptr);
		entity.unbindModel();

	}

} }