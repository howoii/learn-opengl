#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

void main(){
	gl_Position =vec4(position.x, position.z, 0.0, 1.0);
}