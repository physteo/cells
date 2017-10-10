
#pragma once
#include <vector>
#include <fstream> 
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <glm/glm.hpp>

#include "Buffers.h"
#include "utils/TextFileLoader.h"
#include "utils/StringUtils.h"
#include "utils/ObjLoader.h"

namespace tmx { namespace graphics {


	class RawModel
	{
	private:
		// Allocate on the heap, models may be high poly.
		tmx::graphics::Vao* m_vao;
		
	public:
		RawModel(const std::vector<GLuint>& indices,
				 const std::vector<std::vector<GLfloat> >& attributes,
				 const std::vector<GLint>& components)
		{
			m_vao = new tmx::graphics::Vao{ indices, attributes, components };
		}

		RawModel(const std::string& path);
		~RawModel();

		//TODO: write appropriate copy/assignment constructors
		RawModel(const RawModel&) = delete;
		RawModel& operator=(const RawModel&) = delete;

		void bind() const;
		void unbind() const;

		inline size_t getIndicesCount() const { return m_vao->getIndicesCount(); }
		inline size_t getAttributesPerVertexCount() const { return m_vao->getAttributesCount(); }
		inline size_t getAttributeComponentsCount(size_t n) const { return m_vao->getAttributeComponentsCount(n); };

	private:
		void loadModelFromFile(const std::string& path,
			std::vector<std::vector<GLfloat> >& attributes,
			std::vector<GLint>&  components,
			std::vector<GLuint>& indices) const;
	};


}}
