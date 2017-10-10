#include "indexBuffer.h"


namespace sparky {
	namespace graphics {

		IndexBuffer::IndexBuffer(GLushort* data,GLuint count)
			: m_Count(count)
		{

			// generate one buffer and set its ID
			glGenBuffers(1, &m_BufferID);
			// GL_ARRAY_BUFFER means that the buffer will be just an array
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
			// Give the data - size requested is in bytes (not in floats)
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
			// unbind
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void IndexBuffer::bind() const
		{
			// GL_ARRAY_BUFFER means that the buffer will be just an array
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);

		}
		void IndexBuffer::unbind() const
		{
			// unbind
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}


	}
}
