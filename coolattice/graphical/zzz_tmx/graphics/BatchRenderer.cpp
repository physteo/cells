#include "BatchRenderer.h"

namespace tmx { namespace graphics {
	
	BatchRenderer::BatchRenderer()
	{
	}

	BatchRenderer::~BatchRenderer()
	{
		delete m_IndexBuffer;
		glDeleteBuffers(1, &m_VBO);
	}

	// TODO: all the init part should not be done at every frame...
	void BatchRenderer::submit(const TexturedModelGroup* textureModelGroup) 
	{
		RendererMaxObjects = 100; // howto choose appropriate value here? Maybe from the vertexCount...
		GLint vertexCount = textureModelGroup->getTexturedModel().getVertexCount();

		short numberOfGLfloats = 0;
		short numberOfAttributesPerVertex = textureModelGroup->getTexturedModel().getAttributesPerVertexCount();
		for (size_t n = 0; n < numberOfAttributesPerVertex; n++)
		{
			numberOfGLfloats += textureModelGroup->getTexturedModel().getAttributeComponentsCount(n);
		}
		RendererVertexSize = sizeof(GLfloat) * numberOfGLfloats; 
		RendererObjectSize = vertexCount * RendererVertexSize;
		RendererBufferSize = RendererMaxObjects * RendererObjectSize;
		

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, RendererBufferSize, NULL, GL_DYNAMIC_DRAW);
		for (size_t n = 0; n < numberOfAttributesPerVertex; n++)
		{
			GLint components = textureModelGroup->getTexturedModel().getAttributeComponentsCount(n);
			glEnableVertexAttribArray(n);
			glVertexAttribPointer(n, components, GL_FLOAT, GL_FALSE, RendererVertexSize, (const void* )(components * sizeof(GL_FLOAT))  );
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}


	void BatchRenderer::init()
	{
	}

} }