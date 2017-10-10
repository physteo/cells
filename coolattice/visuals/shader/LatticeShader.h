#pragma once

#include "../../graphical/zzz_tmx/ShaderProgram.h"

class LatticeShader : public tmx::graphics::ShaderProgram
{
public:
	LatticeShader();
	~LatticeShader() {}

protected:
	void bindAttributes();
};