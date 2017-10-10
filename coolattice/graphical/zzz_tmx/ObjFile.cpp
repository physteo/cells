#include "ObjFile.h"

namespace tmx { namespace graphics {

	RawModel::RawModel(const std::string& path)
	{
		std::vector<std::vector<GLfloat> > attributes;
		std::vector<GLint> components;
		std::vector<GLuint> indices;

		loadModelFromFile(path, attributes, components, indices);

		m_vao = new tmx::graphics::Vao{ indices, attributes, components };

	}

	RawModel::~RawModel() {
		std::cout << "Deleting RawModel " << this << std::endl;
		delete m_vao;
	}

	void RawModel::bind() const
	{
		m_vao->bindAll();
	}

	void RawModel::unbind() const
	{
		m_vao->unbindAll();
	}


	void RawModel::loadModelFromFile(const std::string& path,
		std::vector<std::vector<GLfloat> >& attributes,
		std::vector<GLint>&  components,
		std::vector<GLuint>& indices) const
	{
		tmx::utils::load_OBJ2(path, attributes, components, indices);
	}



}}



