#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

//layout (location = 2) in vec3 normal;
//layout (location = 3) in vec3 offset;
//layout (location = 4) in vec4 col0;
//layout (location = 5) in vec4 col1;
//layout (location = 6) in vec4 col2;
//layout (location = 7) in vec4 col3;


uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 colour;
out vec2 pass_textureCoords;

void main(void) {
	
	vec4 worldPosition = transformationMatrix * vec4(position, 1.0);
	vec4 positionFromCamera = viewMatrix * worldPosition;


	gl_Position = projectionMatrix * positionFromCamera;
	//gl_Position = transformationMatrix * vec4(position, 1.0);
	
	//colour = vec3(1.0, 0.0, 0.0);
	pass_textureCoords = textureCoords;
}


