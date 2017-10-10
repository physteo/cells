#pragma once

#include <iostream>
#include <vector>
#include <MYGLEW/glew.h>

#include "../maths/maths.h"
#include "../utils/fileutils.h"


namespace sparky { namespace graphics {

    // manage the shader: load the shader from file, sending data to the shader (in the GPU)
    // enabling and disabling the shader, storing uniform locations
    class Shader
    {
    public:
        GLuint m_ShaderID;
        const char* m_VertPath;
        const char* m_FragPath;

    public:
        Shader(const char* vertPath, const char* fragPath);
        ~Shader();


		void setUniform1i(const GLchar* name, int value);
		void setUniform1f(const GLchar* name, float value);
		void setUniform2f(const GLchar* name, const maths::vec2& vector);
		void setUniform3f(const GLchar* name, const maths::vec3& vector);
		void setUniform4f(const GLchar* name, const maths::vec4& vector);
		void setUniformMat4(const GLchar* name, const maths::mat4& matrix);


        void enable() const;
        void disable() const;
    private:
        GLuint load();
		GLint getUniformLocation(const GLchar* name);

    };


}}
