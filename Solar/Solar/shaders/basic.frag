#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 color;

//uniform sampler2D texture0;

void main(){
	//color = texture(texture0, TexCoords);
	color = vec4(0.2f, 0.3f, 0.1f, 1.0f);
}