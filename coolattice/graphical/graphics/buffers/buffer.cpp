#include "buffer.h"


namespace sparky { namespace graphics {  

	Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount)
		: m_ComponentCount(componentCount)
	{

		// generate one buffer and set its ID
		glGenBuffers(1, &m_BufferID);
		// GL_ARRAY_BUFFER means that the buffer will be just an array
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		// Give the data - size requested is in bytes (not in floats)
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Buffer::bind() const
	{
		// GL_ARRAY_BUFFER means that the buffer will be just an array
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

	}
	void Buffer::unbind() const
	{
		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


} }
