#version 400 core

in vec3 position;
in vec2 textureCoords;

//uniform mat4 transformationMatrix = mat4(1.0);

out vec3 colour;
out vec2 pass_textureCoords;

void main(void) {
	gl_Position = vec4(position, 1.0);
	colour = vec3(position.x+0.5, 1.0, position.y+0.5);
	pass_textureCoords = textureCoords;
}