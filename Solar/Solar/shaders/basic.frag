#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 color;

uniform sampler2D diffuse;
uniform sampler2D specular;

void main(){
	color = texture(diffuse, TexCoords);
}