#version 330 core


in vec3 position;
in vec2 textureCoords;
in vec3 normal;

out vec2 pass_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;
const float density = 0.007;
const float gradient = 1.5;

void main(void) {



	vec4 worldPosition = transformationMatrix * vec4(position, 1.0);
	vec4 positionFromCamera = viewMatrix * worldPosition;

	gl_Position = projectionMatrix * positionFromCamera;
	pass_textureCoords = textureCoords;

	surfaceNormal = (transformationMatrix * vec4(normal, 0.0)).xyz;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = ( inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0) ).xyz - worldPosition.xyz;

	float distance = length(positionFromCamera.xyz);
	visibility = exp(-pow(distance*density,gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}

//in vec3 position;
//
//uniform mat4 transformationMatrix = mat4(1.0);
//
//out vec3 colour;
//
//void main(void) {
//	gl_Position = projectionMatrix * transformationMatrix * vec4(position, 1.0);
//	colour = vec3(position.x+0.5, 1.0, position.y+0.5);
//}