#version 330 core

in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform float repeat;
uniform sampler2D texture0;

void main(){
	vec4 texColor = texture(texture0, TexCoords * repeat);
	color = texColor;
}