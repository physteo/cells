#include "LatticeShader.h"


// StaticShader
LatticeShader::LatticeShader() : tmx::graphics::ShaderProgram("visuals/shader/superbasic.vert",
	"visuals/shader/superbasic.frag")
{
	bindAttributes();
	LinkAndValidate();
}

void LatticeShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "scale");
	//bindAttribute(2, "normals");
	//bindAttribute(3, "offset");
}