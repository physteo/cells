#pragma once

#include <MYGLEW/glew.h>
#include <vector>
#include <iostream>

namespace tmx { 	namespace graphics {

	class Ibo
	{
	private:
		GLuint m_iboID;
		size_t m_length;
	public:
		// default constructor useful only if you then move from another ibo
		Ibo() : m_iboID(0), m_length(0) {}
		Ibo(const GLuint* indices, size_t length);
		~Ibo();
		// Ownership is not to be shared
		Ibo(const Ibo&) = delete;
		Ibo& operator=(const Ibo&) = delete;
		// Ownership can be transferred
		Ibo(Ibo&& other);
		Ibo& operator=(Ibo&& other);

		inline GLuint getIboID() const { return m_iboID; }
		inline size_t getLength() const { return m_length; }

		void bind() const;
		void unbind() const;
	private:
		void free();
		void pushData(const GLuint* indices);
	};
	// pi
	class Vbo
	{
	private:
		GLuint m_vboID;
		size_t m_length;
		GLint m_components;
	public:
		// default constructor useful only if you then move from another ibo
		Vbo() : m_vboID(0), m_length(0), m_components(0) {}
		Vbo(const GLfloat* data, size_t length, GLint components);
		~Vbo();
		// Ownership is not to be shared
		Vbo(const Vbo&) = delete;
		Vbo& operator=(const Vbo&) = delete;
		// Ownership can be transferred
		Vbo(Vbo&& other);
		Vbo& operator=(Vbo&& other);

		inline GLuint getVboID() const { return m_vboID; }
		inline size_t getLength() const { return m_length; }
		inline GLint getComponents() const { return m_components; }

		void bind() const;
		void unbind() const;

	private:
		void free();
		void pushData(const GLfloat* data);

	};

	class Vao
	{
	private:
		GLuint m_vaoID;
		size_t m_attributes;
		Ibo* m_ibo;
		std::vector<Vbo*> m_vbos;

	public:
		Vao();
		Vao(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes, const std::vector<GLint>& components);

		~Vao();
		// Ownership is not to be shared
		Vao(const Vao&) = delete;
		Vao& operator=(const Vao&) = delete;
		//TODO: Ownership can be transferred
		Vao(Vao&& other) = delete;
		Vao& operator=(Vao&& other) = delete;

		inline GLuint getVaoID() const { return m_vaoID; }
		inline size_t getAttributesCount() const { return m_attributes;}
		inline size_t getIndicesCount() const { return m_ibo->getLength(); }
		inline size_t getAttributeComponentsCount(size_t n) const { return m_vbos.at(n)->getComponents(); }
		
		void bindAll() const;
		void unbindAll() const;

		void fillVao(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes, const std::vector<GLint>& components);

		// TODO: delete the following (temporary) method
		void tempRender();
	private:
		void assignIbo(Ibo* ibo);
		void addVbo(Vbo* vbo);
		void freeVao();

	};

	

}}

