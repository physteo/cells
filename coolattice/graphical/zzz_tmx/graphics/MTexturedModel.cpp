#include "MTexturedModel.h"

namespace tmx { namespace graphics{

	
	const std::array<GLenum, 6> MTexturedModel::activableTextures = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5};

	MTexturedModel::MTexturedModel(const std::string& pathModel, const std::vector<std::string>& pathsTextures,
			ColorCode image_format, ColorCode internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency)
	{
			std::cout << "Constructor of MTexturedModel " << this << std::endl;
			// TODO make this more than 6
			assert(pathsTextures.size() <= 6);

			// model and texture on the heap because they could be high poly/resolution
			m_rawModel = new RawModel{ pathModel };
			m_textures.reserve(pathsTextures.size());
			for (size_t i = 0; i < pathsTextures.size(); i++) {
				m_textures.push_back(new LTexture{ pathsTextures.at(i), image_format, internal_format, level, border });
			}
			m_shineDamper = shineDamper;
			m_reflectivity = reflectivity;
			m_hasTransparency = hasTransparency;
		}


		void MTexturedModel::fillDataManually(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
			const std::vector<GLint>& components, const std::vector<std::string>& pathsTextures, ColorCode image_format, ColorCode internal_format,
			GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency)
		{
			// TODO make this more than 6
			assert(pathsTextures.size() <= 6);

			m_rawModel = new RawModel{ indices, attributes, components };
			for (size_t i = 0; i < pathsTextures.size(); i++) {
				m_textures.push_back(new LTexture{ pathsTextures.at(i), image_format, internal_format, level, border });
			}
			m_shineDamper = shineDamper;
			m_reflectivity = reflectivity;
			m_hasTransparency = hasTransparency;
		}


		MTexturedModel::MTexturedModel(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
			const std::vector<GLint>& components, const std::vector<std::string>& pathsTextures, ColorCode image_format, ColorCode internal_format,
			GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency) : MTexturedModel()
		{
			// TODO make this more than 6
			assert(pathsTextures.size() <= 6);

			fillDataManually(indices, attributes, components, pathsTextures, image_format, internal_format, level, border, shineDamper, reflectivity, hasTransparency);
		}


		MTexturedModel::~MTexturedModel()
		{

			std::cout << "Destructing MTexturedModel " << this << std::endl;
			delete m_rawModel;
			for (size_t i = 0; i < m_textures.size(); i++) {
				delete m_textures.at(i);
			}
		}

		MTexturedModel::MTexturedModel(MTexturedModel&& other)
		{
			// spilfer
			m_rawModel = other.m_rawModel;
			m_textures = other.m_textures;
			m_shineDamper = other.m_shineDamper;
			m_reflectivity = other.m_reflectivity;
			// set other to default
			other.m_rawModel = nullptr;
			for (size_t i = 0; i < other.m_textures.size(); i++) {
				other.m_textures.at(i) = nullptr;
			}
			
			other.m_shineDamper = 0.0;
			other.m_reflectivity = 0.0;
			std::cout << "Move constructor" << std::endl;
		}
		MTexturedModel& MTexturedModel::operator=(MTexturedModel&& other)
		{
			if (this != &other) {
				// free this
				delete m_rawModel;
				for (size_t i = 0; i < m_textures.size(); i++) {
					delete m_textures.at(i);
				}
				// set this to default
				// spilfer 
				m_rawModel = other.m_rawModel;
				m_textures = other.m_textures;
				m_shineDamper = other.m_shineDamper;
				m_reflectivity = other.m_reflectivity;
				// set other to default
				other.m_rawModel = nullptr;
				for (size_t i = 0; i < other.m_textures.size(); i++) {
					other.m_textures.at(i) = nullptr;
				}
				other.m_shineDamper = 0.0;
				other.m_reflectivity = 0.0;
			}
			return *this;
		}



		void MTexturedModel::bindAll() const
		{
			m_rawModel->bind();
			for (size_t i = 0; i < m_textures.size(); i++) {
				glActiveTexture(MTexturedModel::activableTextures.at(i));
				m_textures.at(i)->bind();
			}
		}
		void MTexturedModel::unbindAll() const
		{
			m_rawModel->unbind();
			for (size_t i = 0; i < m_textures.size(); i++) {
				m_textures.at(i)->unbind();
			}
		}




		// TextureQuad
		MTexturedQuad::MTexturedQuad(GLfloat width, GLfloat height, const std::vector<std::string>& pathsTextures,
			ColorCode image_format, ColorCode internal_format, GLint level, GLint border,
			GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency) : MTexturedModel()
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

			MTexturedModel::fillDataManually(indices, attributes, components, pathsTextures, image_format, internal_format, level, border, shineDamper, reflectivity, hasTransparency);
		}


		MTexturedQuad& MTexturedQuad::operator=(MTexturedQuad&& other)
		{
			m_width = other.m_width;
			m_height = other.m_height;
			MTexturedModel::operator=(std::move(other));
			return *this;
		}


} }