#pragma once
#include <string>
#include <vector>
#include <iostream>

#include <MYGLEW/glew.h>
#include "../utils/fileutils.h"
#include "../maths/maths.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tmx { namespace graphics {


	class ShaderProgram
	{
	protected:
		int programID;
		int vertexShaderID;
		int fragmentShaderID;

	public:
		ShaderProgram(const char* vertexFilename, const char* fragmentShader);
		virtual ~ShaderProgram();
		void start() const;
		void stop() const;

		void loadInt(const GLchar* uniformName, const GLint& value) const;
		void loadFloat(const GLchar* uniformName, const GLfloat& value) const;
		void loadVector(const GLchar* uniformName, const glm::vec3& vector) const;
		void loadBoolean(const GLchar* uniformName, const bool& value) const;
		void loadMatrix(const GLchar* uniformName, const glm::mat4& matrix) const;


	protected:
		void bindAttribute(int attribute, const GLchar* variableName);
		void LinkAndValidate() const;

	private:
		virtual void bindAttributes() = 0;

		int loadShader(const char* filename, int type);
		void cleanUp() const;
		GLint getUniformLocation(const GLchar* uniformName) const;
	};


	class StaticShader : public ShaderProgram
	{
	public:
		StaticShader();
		~StaticShader() {}

	protected:
		void bindAttributes();
	};

	class InstanceStaticShader : public ShaderProgram
	{
	public:
		InstanceStaticShader();
		~InstanceStaticShader() {}

	protected:
		void bindAttributes();
	};

	class SuperSimpleShader : public ShaderProgram
	{
	public:
		SuperSimpleShader();
		~SuperSimpleShader() {}

	protected:
		void bindAttributes();
	};


	class TerrainShader : public ShaderProgram
	{
	public:
		TerrainShader();
		~TerrainShader() {}

	protected:
		void bindAttributes();
	};

	class GrassShader : public ShaderProgram
	{
	public:
		GrassShader();
		~GrassShader() {}

	protected:
		void bindAttributes();
	};


	class GrassInstanceShader : public ShaderProgram
	{
	public:
		GrassInstanceShader();
		~GrassInstanceShader() {}

	protected:
		void bindAttributes();
	};


}}