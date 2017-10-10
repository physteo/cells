#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 offset;
layout (location = 4) in vec4 col0;
layout (location = 5) in vec4 col1;
layout (location = 6) in vec4 col2;
layout (location = 7) in vec4 col3;



out vec2 pass_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

//uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;
const float density = 0.007;
const float gradient = 1.5;

void main(void) {

				//auto translation = glm::translate(glm::mat4(1.0), renderable->position);
				//auto rotation = glm::rotate(glm::mat4(1.0), renderable->angle, renderable->rotation);
				//// first traslation and then rotation... that's because of opengl's fucked up column major order
				//auto transformationMatrix = translation * rotation;

	mat4 translation = mat4(1.0);
	translation[3] = vec4(offset, 1.0);
	//mat4 rotation = 


	mat4 rotation = mat4(0.0);
	rotation[0] = col0;
	rotation[1] = col1;
	rotation[2] = col2;
	rotation[3] = col3;
	
	//vec4 ciaociaone0 = col0;
	//vec4 ciaociaone1 = col1;
	//vec4 ciaociaone2 = col2;
	//vec4 ciaociaone3 = col3;

	mat4 transformationMatrix = translation * rotation;
	
	vec4 worldPosition = transformationMatrix * vec4(position, 1.0);
	vec4 positionFromCamera = viewMatrix * worldPosition;

	gl_Position = projectionMatrix * positionFromCamera;
	pass_textureCoords = textureCoords;

	surfaceNormal = (transformationMatrix * vec4(0.0,1.0,0.0,0.0)).xyz;
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
