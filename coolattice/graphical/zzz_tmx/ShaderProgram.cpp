#include "ShaderProgram.h"

namespace tmx { namespace graphics {

	ShaderProgram::ShaderProgram(const char* vertexFilename, const char* fragmentFilename)
	{
		vertexShaderID = loadShader(vertexFilename, GL_VERTEX_SHADER);
		fragmentShaderID = loadShader(fragmentFilename, GL_FRAGMENT_SHADER);
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);

		// the stuff below goes in the costructor of each derived shader class
		// Why? because the attributes needs to be binded before the program is linked,
		// and different derived shaders might have different attributes.

		//bindAttributes();
		//glLinkProgram(programID);
		//glValidateProgram(programID);
	}
	
	
	ShaderProgram::~ShaderProgram()
	{
		cleanUp();
	}
	
	
	int ShaderProgram::loadShader(const char* filename, int type)
	{
		std::string sourceString = sparky::FileUtils::read_file(filename);
		const char* source = sourceString.c_str();

		GLuint shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);
		GLint status;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(shaderID, length, &length, &error[0]);
			std::cout << "Failed to compile shader" 
						<< std::endl << &error[0] << std::endl;

			glDeleteShader(shaderID);
			return 0;
		}


		return shaderID;
	}


	void ShaderProgram::start() const
	{
		glUseProgram(programID);
	}

	void ShaderProgram::stop() const
	{
		glUseProgram(0);
	}



	GLint ShaderProgram::getUniformLocation(const GLchar* uniformName) const
	{
		return glGetUniformLocation(programID, uniformName);
	}

	void ShaderProgram::loadInt(const GLchar* uniformName, const GLint& value) const
	{
		glUniform1i(getUniformLocation(uniformName), value);
	}

	void ShaderProgram::loadFloat(const GLchar* uniformName, const GLfloat& value) const
	{
		glUniform1f(getUniformLocation(uniformName), value);
	}

	void ShaderProgram::loadVector(const GLchar* uniformName, const glm::vec3& vector) const
	{
		glUniform3f(getUniformLocation(uniformName), vector.x, vector.y, vector.z);
	}

	void ShaderProgram::loadBoolean(const GLchar* uniformName, const bool& value) const
	{
		float toLoad = 0;
		if (value) { toLoad = 1; }
		glUniform1f(getUniformLocation(uniformName), toLoad);
	}

	void ShaderProgram::loadMatrix(const GLchar* uniformName, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::bindAttribute(int attribute, const GLchar* variableName)
	{
		glBindAttribLocation(programID, attribute, variableName);
	}


	void ShaderProgram::cleanUp() const
	{
		stop();
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteProgram(programID);
	}


	void ShaderProgram::LinkAndValidate() const
	{
		glLinkProgram(programID);
		glValidateProgram(programID);
	}


	// StaticShader
	StaticShader::StaticShader() : ShaderProgram("src/shaders/tmx.vert",
		"src/shaders/tmx.frag")
	{
		bindAttributes();
		LinkAndValidate();
	}

	void StaticShader::bindAttributes() {
		bindAttribute(0, "position");
		bindAttribute(1, "textureCoords");
		bindAttribute(2, "normals");
	}

	// InstanceStaticShader
	InstanceStaticShader::InstanceStaticShader() : ShaderProgram("C:/Users/matte/source/repos/coolattice/coolattice/graphical/shaders/instance.vert",
		"C:/Users/matte/source/reposcoolattice/graphical/shaders/instance.frag")
	{
		bindAttributes();
		LinkAndValidate();
	}

	void InstanceStaticShader::bindAttributes() {
		bindAttribute(0, "position");
		bindAttribute(1, "textureCoords");
		bindAttribute(2, "normals");
		bindAttribute(3, "offset");
	}

	// SuperSimpleShader
	SuperSimpleShader::SuperSimpleShader() : ShaderProgram("src/shaders/superbasic.vert",
		"src/shaders/superbasic.frag")
	{
		bindAttributes();
		LinkAndValidate();
	}

	void SuperSimpleShader::bindAttributes() {
		bindAttribute(0, "position");
		//bindAttribute(1, "colour");
	}


	// TerrainShader
	TerrainShader::TerrainShader() : ShaderProgram("src/shaders/terrain.vert",
		"src/shaders/terrain.frag")
	{
		bindAttributes();
		LinkAndValidate();
	}

	void TerrainShader::bindAttributes() {
		bindAttribute(0, "position");
		bindAttribute(1, "textureCoords");
		bindAttribute(2, "normals");
	}



	// grassShader
	GrassShader::GrassShader() : ShaderProgram("src/shaders/grass.vert",
		"src/shaders/grass.frag")
	{
		bindAttributes();
		LinkAndValidate();
	}

	void GrassShader::bindAttributes() {
		bindAttribute(0, "position");
		bindAttribute(1, "textureCoords");
		bindAttribute(2, "normals");
	}


	// grassInstanceShader
	GrassInstanceShader::GrassInstanceShader() : ShaderProgram("src/shaders/grassInstance.vert",
		"src/shaders/grassInstance.frag")
	{
		bindAttributes();
		LinkAndValidate();
	}

	void GrassInstanceShader::bindAttributes() {
		bindAttribute(0, "position");
		bindAttribute(1, "textureCoords");
		bindAttribute(2, "normals");
	}


}}