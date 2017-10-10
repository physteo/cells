#version 330 core

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D texture0;


uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

void main(void){

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, 0.1);
	vec3 diffuse = brightness * lightColour;


	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);
	float dampedFactor = pow(specularFactor, shineDamper);
	
	vec3 finalSpecular = dampedFactor * reflectivity * lightColour;
	vec4 textureColor =  texture(texture0, pass_textureCoords);
	
	if(textureColor.a < 0.5) {
		discard;
	}
	
	out_Color = vec4(diffuse, 1.0) * textureColor + vec4(finalSpecular, 1.0);
	out_Color = mix(vec4(skyColor,1.0),out_Color, visibility);	
	
}

//in vec3 colour;
//
//out vec4 out_Color;
//
//void main(void){
//	out_Color = vec4(colour,1.0);
//
//}