#version 330 core

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

void main(void){

	
	vec2 blendMapCoords = pass_textureCoords;
	blendMapCoords.x = blendMapCoords.x/10.0;
	blendMapCoords.y = blendMapCoords.y/10.0;
	

	vec4 blendMapColour = texture(texture4, blendMapCoords);
	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	//vec2 tiledCoords = pass_textureCoordinates * 40;
	vec4 backgroundTextureColour = texture(texture0, pass_textureCoords) * backTextureAmount;
	vec4 rTextureColour          = texture(texture1,pass_textureCoords) * blendMapColour.r;
	vec4 gTextureColour			 = texture(texture2,pass_textureCoords) * blendMapColour.g;
	vec4 bTextureColour			 = texture(texture3,pass_textureCoords) * blendMapColour.b;

	
	vec4 totalColour = backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;

	//vec4 totalColour = texture(texture0, pass_textureCoords);

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
	out_Color = vec4(diffuse, 1.0) * totalColour + vec4(finalSpecular, 1.0);
	out_Color = mix(vec4(skyColor,1.0),out_Color, visibility);	

}
