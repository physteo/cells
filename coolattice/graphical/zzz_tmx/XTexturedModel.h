#pragma once
#include <MYGLEW/glew.h>
#include "Buffers.h"
#include "LTexture.h"

#include "ObjFile.h"

namespace tmx { namespace graphics {


	class MyTexturedModel
	{
	private:
		const RawModel* m_rawModel;
		const LTexture* m_texture;
		GLfloat m_shineDamper;
		GLfloat m_reflectivity;

	protected:
		MyTexturedModel() {}
	public:
		MyTexturedModel(const std::string& pathModel, const std::string& pathTexture,
			GLenum image_format, GLint internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity);
		
		MyTexturedModel(GLuint width, GLuint height, const std::string& pathTexture,
			GLenum image_format, GLint internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity);

		~MyTexturedModel();

		// Ownership cannot be shared
		MyTexturedModel(const MyTexturedModel& other) = delete;
		MyTexturedModel& operator=(const MyTexturedModel&) = delete;

		// Ownership can be transferred
		MyTexturedModel(MyTexturedModel&& other);
		MyTexturedModel& operator=(MyTexturedModel&& other);

		inline GLfloat getShineDamper() const { return m_shineDamper; }
		inline GLfloat getReflectivity() const { return m_reflectivity; }

		void bindAll() const;
		void unbindAll() const;

		inline size_t getVertexCount() const { return m_rawModel->getIndicesCount(); }

	};


	// TODO implement this
	//class MyTexturedQuad : public MyTexturedModel
	//{
	//public:
	//	MyTexturedQuad(GLuint width, GLuint height)
	//	{
	//		std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};
	//		std::vector<GLfloat> attributes{};
	//		std::vector<GLint> components;
	//
	//		m_rawModel = new RawModel{ pathModel };
	//		m_texture = new LTexture{ pathTexture, GL_BGR, GL_RGB, level, border };
	//
	//	}
	//
	//
	//};



	// Deprecated
	class XTexturedModel
	{
	public:
		Vao* const vao;
		LTexture* const texture;
	public:
		XTexturedModel(Vao* const _vao, LTexture* const _texture) :
			vao(_vao), texture(_texture) {}

		void bindAll() const;
		void unbindAll() const;

	};


} }