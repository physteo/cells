#include "XTexturedModel.h"

namespace tmx { namespace graphics {


	MyTexturedModel::MyTexturedModel(const std::string& pathModel, const std::string& pathTexture,
		GLenum image_format, GLint internal_format, GLint level, GLint border,
		GLfloat shineDamper, GLfloat reflectivity)
	{
		std::cout << "Constructor of TexturedModel " << this << std::endl;

		// model and texture on the heap because they could be high poly/resolution
		m_rawModel = new RawModel{ pathModel };
		m_texture = new LTexture{ pathTexture, GL_BGR, GL_RGB, level, border };

		m_shineDamper = shineDamper;
		m_reflectivity = reflectivity;
	}

	MyTexturedModel::MyTexturedModel(GLuint width, GLuint height, const std::string& pathTexture,
		GLenum image_format, GLint internal_format, GLint level, GLint border,
		GLfloat shineDamper, GLfloat reflectivity)
	{
		// TODO: create indices and all..
		std::vector<GLuint> indices{0, 1, 2, 2, 3, 0};
		std::vector<GLfloat> positions{ -0.5f * width, 0.5f * height, 0.0f,
										-0.5f * width,-0.5f * height, 0.0f,
										 0.5f * width,-0.5f * height, 0.0f,
										 0.5f * width, 0.5f * height, 0.0f, };

		std::vector<GLfloat> textureCoords = {
			0,0,
			0,1,
			1,1,
			1,0,
		};


		std::vector<std::vector<GLfloat> > attributes{ positions, textureCoords };
		std::vector<GLint> components = { 3,2 };


		m_rawModel = new RawModel{ indices, attributes, components };
		m_texture = new LTexture{ pathTexture, GL_BGR, GL_RGB, level, border };

		m_shineDamper = shineDamper;
		m_reflectivity = reflectivity;
	}


	MyTexturedModel::~MyTexturedModel()
	{

		std::cout << "Destructing TexturedModel " << this << std::endl;
		delete m_rawModel;
		delete m_texture;
	}

	MyTexturedModel::MyTexturedModel(MyTexturedModel&& other)
	{
		// spilfer
		m_rawModel = other.m_rawModel;
		m_texture = other.m_texture;
		m_shineDamper = other.m_shineDamper;
		m_reflectivity = other.m_reflectivity;
		// set other to default
		other.m_rawModel = nullptr;
		other.m_texture = nullptr;
		other.m_shineDamper = 0.0;
		other.m_reflectivity = 0.0;
		std::cout << "Move constructor" << std::endl;
	}
	MyTexturedModel& MyTexturedModel::operator=(MyTexturedModel&& other)
	{
		if (this != &other) {
			// free this
			delete m_rawModel;
			delete m_texture;
			// set this to default
			// spilfer 
			m_rawModel = other.m_rawModel;
			m_texture = other.m_texture;
			m_shineDamper = other.m_shineDamper;
			m_reflectivity = other.m_reflectivity;
			// set other to default
			other.m_rawModel = nullptr;
			other.m_texture = nullptr;
			other.m_shineDamper = 0.0;
			other.m_reflectivity = 0.0;
		}
		return *this;
	}

	//MyTexturedModel::MyTexturedModel(const MyTexturedModel& other)
	//{
	//	m_rawModel = other.m_rawModel;
	//	m_texture = other.m_texture;
	//	m_shineDamper = other.m_shineDamper;
	//	m_reflectivity = other.m_reflectivity;
	//}

	void MyTexturedModel::bindAll() const
	{
		m_rawModel->bind();
		m_texture->bind();
	}
	void MyTexturedModel::unbindAll() const
	{
		m_rawModel->unbind();
		m_texture->unbind();
	}

	// Deprecated
	void XTexturedModel::bindAll() const {
		vao->bindAll();
		texture->bind();
	}
	void XTexturedModel::unbindAll() const {
		vao->unbindAll();
		texture->unbind();
	}


}}