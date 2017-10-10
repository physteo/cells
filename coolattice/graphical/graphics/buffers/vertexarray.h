#pragma once

#include <vector>
#include <MYGLEW\glew.h>

#include "buffer.h"

namespace sparky { namespace graphics {

	class VertexArray
	{
	private:
		GLuint m_ArrayID;
		// deleting the buffers will be the job of the vertex array now
		std::vector<Buffer*> m_Buffers;

	public:
		// generates a vertex array, sets the id
		VertexArray();
		~VertexArray();

		// index here will be the "location" inside the shaders
		void addBuffer(Buffer* buffer, GLuint index);
		void bind() const;
		void unbind() const;
	};


} }