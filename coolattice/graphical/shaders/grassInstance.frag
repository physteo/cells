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

	//o/ = I have taken away the reflectivity for grass, cause grass is not reflective and 
	//o/ eats up speed
	//o/vec3 unitVectorToCamera = normalize(toCameraVector);
	//o/vec3 lightDirection = -unitLightVector;
	//o/vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	//o/
	//o/float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	//o/specularFactor = max(specularFactor, 0.0);
	//o/float dampedFactor = pow(specularFactor, shineDamper);
	//o/
	//o/vec3 finalSpecular = dampedFactor * reflectivity * lightColour;

	vec4 textureColor =  texture(texture0, pass_textureCoords);
	
	if(textureColor.a < 0.5) {
		discard;
	}
	
	//o/out_Color = vec4(diffuse, 1.0) * textureColor + vec4(finalSpecular, 1.0);
	out_Color = vec4(diffuse, 1.0) * textureColor;
	out_Color = mix(vec4(skyColor,1.0),out_Color, visibility);	
	
}