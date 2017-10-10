#include "Buffers.h"

namespace tmx { 	namespace graphics {


	// Ibo
	Ibo::Ibo(const GLuint* indices, size_t length) : m_iboID(0), m_length(length) {
		pushData(indices);
	}
	Ibo::~Ibo()
	{
		std::cout << "Deleting Ibo " << this << " ..." << std::endl;
		free();
	}

	Ibo::Ibo(Ibo&& other)
	{
		// free this
		// TODO: is it safe to delete this? cause I dont know what the iboID is now..
		// but I guess it's the defaulted 0 (from the default constructor)
		//free();
		// pilfer other's resource
		m_iboID = other.m_iboID;
		m_length = other.m_length;
		// set other to default, but do not free the memory on the GPU
		// because it is now owned by this.
		other.m_iboID = 0;
		other.m_length = 0;
		std::cout << "Move constructor of " << this  << std::endl;
	}

	Ibo& Ibo::operator=(Ibo&& other)
	{
		if (this != &other)
		{
			// free this
			free();
			// spilfer other into this
			m_iboID = other.m_iboID;
			m_length = other.m_length;
			// set other to default, but do not free the memory on the GPU
			// because it is now owned by this.
			other.m_iboID = 0;
			other.m_length = 0;
		}
		std::cout << "Move assignment of " << this << std::endl;

		return *this;
	}


	void Ibo::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	}
	void Ibo::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Ibo::pushData(const GLuint* indices) {
		glGenBuffers(1, &m_iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_length * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Ibo::free() {
		std::cout << "...freeing IBO" << this << std::endl;
		//GLuint id = ibo.getIboID();
		glDeleteBuffers(1, &m_iboID);
	}


	// Vbo
	Vbo::Vbo(const GLfloat* data, size_t length, GLint components)
		: m_vboID(0), m_length(length), m_components(components)
	{
		pushData(data);
	}


	Vbo::~Vbo()
	{
		std::cout << "Deleting Vbo " << this << " ..." << std::endl;
		free();
	};


	Vbo::Vbo(Vbo&& other)
	{
		// free this
		// TODO: is it safe to delete this? cause I dont know what the iboID is now..
		// but I guess it's the defaulted 0 (from the default constructor)
		free();
		// pilfer other's resource
		m_vboID = other.m_vboID;
		m_length = other.m_length;
		m_components = other.m_components;
		// set other to default, but do not free the memory on the GPU
		// because it is now owned by this.
		other.m_vboID = 0;
		other.m_length = 0;
		other.m_components = 0;
		std::cout << "Move constructor of " << this << std::endl;
	}

	Vbo& Vbo::operator=(Vbo&& other)
	{
		if (this != &other)
		{
			// free this
			free();
			// spilfer other into this
			m_vboID = other.m_vboID;
			m_length = other.m_length;
			m_components = other.m_components = 0;
			// set other to default, but do not free the memory on the GPU
			// because it is now owned by this.
			other.m_vboID = 0;
			other.m_length = 0;
			other.m_components = 0;
		}
		std::cout << "Move assignment of " << this << std::endl;

		return *this;
	}


	void Vbo::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	}

	void Vbo::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Vbo::pushData(const GLfloat* data)
	{
		// read data from the array and push it into GPU
		glGenBuffers(1, &m_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, m_length * sizeof(GLfloat), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Vbo::free()
	{
		std::cout << "...freeing VBO" << this << std::endl;
		glDeleteBuffers(1, &m_vboID);
	}




	// VAO
	Vao::Vao()
	{
		// create VAO
		glGenVertexArrays(1, &m_vaoID);
		m_attributes = 0;
		m_ibo = nullptr;
	}

	Vao::Vao(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes, const std::vector<GLint>& components) : Vao()
	{
		fillVao(indices, attributes, components);
	}

	void Vao::fillVao(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes, const std::vector<GLint>& components)
	{
		if (m_ibo == nullptr)
		{
			// create Ibo on the heap (free when freeing the VAO) and assign this Ibo to the Vao
			Ibo* ibo = new Ibo(&indices[0], indices.size());
			assignIbo(ibo);

			// create all vbos on the heap( remember to free at the destructor) and add all of them to the Vao
			for (size_t j = 0; j < attributes.size(); j++)
			{
				Vbo* vbo = new Vbo(&attributes.at(j)[0], attributes.at(j).size(), components.at(j));
				addVbo(vbo);
				m_vbos.push_back(vbo);
			}
		}
		else {
			throw std::runtime_error("Vao was already filled with data!");
		}
	}

	Vao::~Vao()
	{
		std::cout << "Deleting VAO " << this << " ..." << std::endl;
		for (size_t a = 0; a < m_vbos.size(); a++)
		{
			delete m_vbos.at(a);
		}
		delete m_ibo;
		freeVao();
	}


	void Vao::assignIbo(Ibo* ibo)
	{

		if (m_ibo == nullptr) { m_ibo = ibo; }
		else { throw std::runtime_error("Ibo is already present for Vao."); }
	}

	void Vao::addVbo(Vbo* vbo)
	{
		glBindVertexArray(m_vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vbo->getVboID());
		glVertexAttribPointer(m_attributes, vbo->getComponents(), GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		m_attributes++;
	}

	void Vao::bindAll() const
	{
		glBindVertexArray(m_vaoID);
		for (size_t i = 0; i < m_attributes; i++) { glEnableVertexAttribArray(i); }
		m_ibo->bind();
	}

	void Vao::unbindAll() const {
		m_ibo->unbind();
		for (size_t i = 0; i < m_attributes; i++) { glDisableVertexAttribArray(i); }
		glBindVertexArray(0);
	}

	void Vao::freeVao()
	{
		std::cout << "...freeing VAO " << this << std::endl;
		glDeleteVertexArrays(1, &m_vaoID);
	}

	void Vao::tempRender()
	{
		this->bindAll();
		glDrawElements(GL_TRIANGLES, this->m_ibo->getLength(), GL_UNSIGNED_INT, nullptr);
		this->unbindAll();
	}


}}