#pragma once

#include "../Buffers.h"
#include "Renderer.h"

namespace tmx { namespace graphics {

	class BatchRenderer : public Renderer
	{
		Vao m_VAO;
		Ibo* m_IndexBuffer;
		GLsizei m_IndexCount;
		GLuint m_VBO;

		GLint RendererMaxObjects;
		GLint RendererVertexSize;
		GLint RendererObjectSize;
		GLint RendererBufferSize;
		GLint RendererIndicesSize;

	public:
		BatchRenderer();
		~BatchRenderer();
		void submit(const TexturedModelGroup* textureModelGroup);
		void flush(){}
	private:
		void init();
	};

} }