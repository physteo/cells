#pragma once
#include <MYGLEW/glew.h>

namespace sparky { namespace graphics { 

	// This class will contain info for vertices (color, position, etc)
	class Buffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_ComponentCount; // vec2, vec3 or vec4?
	public:
		Buffer(GLfloat* data, GLsizei count, GLuint componentCount);
		 
		void bind() const;
		void unbind() const;

		inline GLuint getComponentCount() const { return m_ComponentCount; }
	};


}
}