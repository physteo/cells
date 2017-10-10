#pragma once

#include <MYGLEW/glew.h>
#include "Renderable.h"

namespace tmx { namespace graphics {

	class Renderer
	{
	public:
		virtual void submit(const Renderable* renderable) = 0;
		virtual void flush() = 0;
	};

} }