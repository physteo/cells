#include "TexturedModel.h"

namespace tmx {
	namespace graphics {


		TexturedModel::TexturedModel(const std::string& pathModel, const std::string& pathTexture,
			ColorCode image_format, ColorCode internal_format, GLint level, GLint border,
			GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency)
		{
			std::cout << "Constructor of TexturedModel " << this << std::endl;

			// model and texture on the heap because they could be high poly/resolution
			m_rawModel = new RawModel{ pathModel };
			m_texture = new LTexture{ pathTexture, image_format, internal_format, level, border };

			m_shineDamper = shineDamper;
			m_reflectivity = reflectivity;
			m_hasTransparency = hasTransparency;
		}


		// Delegated to TexturedQuad
		/*
		TexturedModel::TexturedModel(GLuint width, GLuint height, const std::string& pathTexture,
			ColorCode image_format, ColorCode internal_format, GLint level, GLint border,
			GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency)
		{
			std::vector<GLuint> indices{ 0, 1, 2, 2, 3, 0 };
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

			std::vector<GLfloat> normals = {0,0,1, 0,0,1, 0,0,1, 0,0,1};


			std::vector<std::vector<GLfloat> > attributes{ positions, textureCoords, normals };
			std::vector<GLint> components = { 3,2,3 };


			m_rawModel = new RawModel{ indices, attributes, components };
			m_texture = new LTexture{ pathTexture, image_format, internal_format, level, border };

			m_shineDamper = shineDamper;
			m_reflectivity = reflectivity;
			m_hasTransparency = hasTransparency;
		}
		*/

		void TexturedModel::fillDataManually(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
			const std::vector<GLint>& components, const std::string& pathTexture, ColorCode image_format, ColorCode internal_format,
			GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency)
		{
			m_rawModel = new RawModel{ indices, attributes, components };
			m_texture = new LTexture{ pathTexture, image_format, internal_format, level, border };

			m_shineDamper = shineDamper;
			m_reflectivity = reflectivity;
			m_hasTransparency = hasTransparency;
		}


		TexturedModel::TexturedModel(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
			const std::vector<GLint>& components, const std::string& pathTexture, ColorCode image_format, ColorCode internal_format,
			GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency) : TexturedModel()
		{
			fillDataManually(indices, attributes, components, pathTexture, image_format, internal_format, level, border, shineDamper, reflectivity, hasTransparency);
		}


		TexturedModel::~TexturedModel()
		{

			std::cout << "Destructing TexturedModel " << this << std::endl;
			delete m_rawModel;
			delete m_texture;
		}

		TexturedModel::TexturedModel(TexturedModel&& other)
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
		TexturedModel& TexturedModel::operator=(TexturedModel&& other)
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



		void TexturedModel::bindAll() const
		{
			m_rawModel->bind();
			m_texture->bind();
		}
		void TexturedModel::unbindAll() const
		{
			m_rawModel->unbind();
			m_texture->unbind();
		}


		// TextureQuad
		TexturedQuad::TexturedQuad(GLuint width, GLuint height, const std::string& pathTexture,
			ColorCode image_format, ColorCode internal_format, GLint level, GLint border,
			GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency) : TexturedModel()
		{
			m_width = width;
			m_height = height;
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
			std::vector<GLfloat> normals = { 0,0,1, 0,0,1, 0,0,1, 0,0,1 };

			std::vector<GLuint> indices{ 0, 1, 2, 2, 3, 0 };
			std::vector<std::vector<GLfloat> > attributes{ positions, textureCoords, normals };
			std::vector<GLint> components = { 3,2,3 };

			TexturedModel::fillDataManually(indices, attributes, components, pathTexture, image_format, internal_format, level, border, shineDamper, reflectivity, hasTransparency);
		}


		TexturedQuad& TexturedQuad::operator=(TexturedQuad&& other)
		{
			m_width = other.m_width;
			m_height = other.m_height;
			TexturedModel::operator=(std::move(other));
			return *this;
		}




	}
}