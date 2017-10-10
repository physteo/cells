#version 330 core

in vec3 colour;
in vec2 pass_textureCoords;

out vec4 out_Color;

uniform sampler2D texture0;
//uniform sampler2D texture1;

void main(void){

	vec4 texture0Color =  texture(texture0, pass_textureCoords);
	//vec4 texture1Color =  texture(texture1, pass_textureCoords);

	out_Color = texture0Color;

	//out_Color = vec4(colour,1.0);
}