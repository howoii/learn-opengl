#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;
uniform vec3 offsets[20];

out vec2 TexCoords;

void main(){
	vec3 offset = offsets[gl_InstanceID];
	gl_Position = projection * view * model * vec4(position + offset, 1.0);
	TexCoords = texCoords;
}