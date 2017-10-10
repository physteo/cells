#pragma once
#include "../maths/maths.h"

#include "XTexturedModel.h"
#include "Entity.h"
#include "ShaderProgram.h"

#include <MYGLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <FreeImage.h>

#include <vector>
#include <iostream>
namespace tmx { 	namespace renderEngine {
	

	class Renderer {
	public:
		void prepare();
		//void render(const tmx::graphics::RawModel&  model);
		void RenderEntity(const tmx::entities::Entity& entity, tmx::graphics::StaticShader& shader);
		
		// deprecated
		void Renderer::renderVao(const tmx::graphics::Vao& vao);
		void RenderXTexturedModel(const tmx::graphics::XTexturedModel& xTexturedModel);
	};


} }