#version 330 core
layout (location = 0) in vec3 position;

layout (std140) uniform Transform
{
	mat4 view;
	mat4 projection;
};

out vec3 TexCoords;

uniform mat4 model;

void main()
{
    vec4 pos =   projection * view * model * vec4(position, 1.0);
	gl_Position = pos;
    TexCoords = position;
}