#pragma once

#include "renderer2d.h"
#include "buffers/vertexarray.h"
namespace sparky { namespace graphics {

	// maximum number of sprites renderable
#define RENDERER_MAX_SPRITES	10000
	// size in bytes of the data struct needed for each vertex
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
	// Size in byes for a single sprite
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
	// size in bytes of the whole buffer containing all the sprites
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6

	class BatchRenderer2D : public Renderer2D
	{
	//private:
	//	VertexArray m_VAO;
	//	IndexBuffer* m_IBO;
	//	GLsizei m_IndexCount;
	//	GLuint m_VBO;
	//public:
	//	virtual void submit(const Renderable2D* renderable) override;
	//	virtual void flush() override;
	//private:
	//	void init();
	};


} }