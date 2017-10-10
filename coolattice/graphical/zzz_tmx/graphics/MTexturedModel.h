#pragma once
#include "../ObjFile.h"
#include "../LTexture.h"
#include <array>
#include <assert.h>

namespace tmx { namespace graphics {

	static std::vector<int> a;

	class MTexturedModel
	{
	private:
		const RawModel* m_rawModel;
		std::vector<LTexture*> m_textures;
		GLfloat m_shineDamper;
		GLfloat m_reflectivity;
		bool m_hasTransparency;

		// maximum number of supported textures is 6 for now
		static const std::array<GLenum, 6> activableTextures;

	protected:
		MTexturedModel() : m_rawModel(nullptr), m_textures(), m_shineDamper(0.0f), m_reflectivity(0.0f), m_hasTransparency(false) {}

		void fillDataManually(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
			const std::vector<GLint>& components, const std::vector< std::string>& pathsTextures, ColorCode image_format, ColorCode internal_format,
			GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);
	public:
		MTexturedModel(const std::string& pathModel, const std::vector<std::string>& pathsTextures,
			ColorCode image_format, ColorCode internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);

		MTexturedModel(const std::vector<GLuint>& indices, const std::vector<std::vector<GLfloat> >& attributes,
			const std::vector<GLint>& components, const std::vector<std::string>& pathsTextures, ColorCode image_format, ColorCode internal_format,
			GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency = false);

		virtual ~MTexturedModel();

		// Ownership cannot be shared
		MTexturedModel(const MTexturedModel& other) = delete;
		MTexturedModel& operator=(const MTexturedModel&) = delete;

		// Ownership can be transferred
		MTexturedModel(MTexturedModel&& other);
		MTexturedModel& operator=(MTexturedModel&& other);

		inline GLfloat getShineDamper() const { return m_shineDamper; }
		inline GLfloat getReflectivity() const { return m_reflectivity; }
		inline bool hasTransparency() const { return m_hasTransparency; }

		void bindAll() const;
		void unbindAll() const;

		inline size_t getIndicesCount() const { return m_rawModel->getIndicesCount(); }
		inline size_t getAttributesPerVertexCount() const { return m_rawModel->getAttributesPerVertexCount(); }
		inline size_t getAttributeComponentsCount(size_t n) const { return m_rawModel->getAttributeComponentsCount(n); };
		inline size_t getNumberOfTextures() const { return m_textures.size(); }

		// TODO: change this bullshit...
		inline virtual GLfloat getWidth() const { return 0.0; }
		inline virtual GLfloat getHeight() const { return 0.0; }

	};



	class MTexturedQuad : public MTexturedModel
	{
	private:
		GLfloat m_width;
		GLfloat m_height;
	public:
		MTexturedQuad(GLfloat width, GLfloat height, const std::vector<std::string>& pathsTextures,
			ColorCode image_format, ColorCode internal_format, GLint level, GLint border,
			GLfloat shineDamper, GLfloat reflectivity, bool hasTransparency=true);

		// Ownership cannot be shared
		MTexturedQuad(const MTexturedQuad& other) = delete;
		MTexturedQuad& operator=(const MTexturedQuad&) = delete;

		// Ownership can be transferred
		MTexturedQuad(MTexturedQuad&& other) : m_width(other.m_width), m_height(other.m_height), MTexturedModel(std::move(other)) {}
		MTexturedQuad& operator=(MTexturedQuad&& other);

		inline GLfloat getWidth() const override { return m_width; }
		inline GLfloat getHeight() const override { return m_height; }

	};


} }