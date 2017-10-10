#pragma once

#pragma once
#include <MYGLEW/glew.h>
#include "../Buffers.h"
#include "../LTexture.h"
#include "../ObjFile.h"



namespace tmx {
	namespace graphics {



		class TexturedModel
		{
		private:
			const RawModel* m_rawModel;
			const LTexture* m_texture;
			GLfloat m_shineDamper;
			GLfloat m_reflectivity;
			bool m_hasTransparency;
			
		protected:
			TexturedModel() : m_rawModel(nullptr), m_texture(nullptr), m_shineDamper(0.0f), m_reflectivity(0.0f), m_hasTransparency(false) {}

			void fillDataManually(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
				const std::vector<GLint>& components, const std::string& pathTexture, ColorCode image_format, ColorCode internal_format,
				GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);
		public:
			TexturedModel(const std::string& pathModel, const std::string& pathTexture,
				ColorCode image_format, ColorCode internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);

			TexturedModel(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
				const std::vector<GLint>& components, const std::string& pathTexture, ColorCode image_format, ColorCode internal_format,
				GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);

			// Delegated to TexturedQuad
			//TexturedModel(GLuint width, GLuint height, const std::string& pathTexture,
			//	ColorCode image_format, ColorCode internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);
			virtual ~TexturedModel();

			// Ownership cannot be shared
			TexturedModel(const TexturedModel& other) = delete;
			TexturedModel& operator=(const TexturedModel&) = delete;

			// Ownership can be transferred
			TexturedModel(TexturedModel&& other);
			TexturedModel& operator=(TexturedModel&& other);

			inline GLfloat getShineDamper() const { return m_shineDamper; }
			inline GLfloat getReflectivity() const { return m_reflectivity; }
			inline bool hasTransparency() const { return m_hasTransparency; }

			void bindAll() const;
			void unbindAll() const;

			inline size_t getIndicesCount() const { return m_rawModel->getIndicesCount(); }
			inline size_t getAttributesPerVertexCount() const { return m_rawModel->getAttributesPerVertexCount(); }
			inline size_t getAttributeComponentsCount(size_t n) const { return m_rawModel->getAttributeComponentsCount(n); };

			// TODO: change this bullshit...
			inline virtual GLfloat getWidth() const { return 0.0; }
			inline virtual GLfloat getHeight() const { return 0.0; }

		};



		class TexturedQuad : public TexturedModel
		{
		private:
			GLfloat m_width;
			GLfloat m_height;
		public:
			TexturedQuad(GLuint width, GLuint height, const std::string& pathTexture, ColorCode image_format,
				ColorCode internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);

			// Ownership cannot be shared
			TexturedQuad(const TexturedQuad& other) = delete;
			TexturedQuad& operator=(const TexturedQuad&) = delete;

			// Ownership can be transferred
			TexturedQuad(TexturedQuad&& other) : m_width(other.m_width), m_height(other.m_height), TexturedModel(std::move(other)) {}
			TexturedQuad& operator=(TexturedQuad&& other);

			inline GLfloat getWidth() const override { return m_width; }
			inline GLfloat getHeight() const override { return m_height; }

		};



		//		class TexturedModelGroup
		//		{
		//		private:
		//			TexturedModel* texturedModel;
		//			std::vector<Renderable> objects;
		//
		//		public:
		//
		//
		//			TexturedModelGroup(const std::string& pathModel, const std::string& pathTexture,
		//				GLenum image_format, GLint internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity)
		//			{
		//				texturedModel = new TexturedModel(pathModel, pathTexture,
		//					image_format,  internal_format,  level,  border,  shineDamper,  reflectivity);
		//			}
		//
		//			TexturedModelGroup(GLuint width, GLuint height, const std::string& pathTexture,
		//				GLenum image_format, GLint internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity)
		//			{
		//				texturedModel = new TexturedModel(width, height, pathTexture,
		//					image_format, internal_format, level, border, shineDamper, reflectivity);
		//			}
		//
		//			~TexturedModelGroup()
		//			{
		//				delete texturedModel;
		//			}
		//
		//			// TODO: delete copy/move constructors and assignments
		//
		//			inline std::vector<Renderable>& objectsVector() { return objects; }
		//			inline const TexturedModel& getTexturedModel() const { return *texturedModel; }
		//
		//		};
		//
		//	}
	}
}