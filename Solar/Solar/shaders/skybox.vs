#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

out vec3 TexCoords;

uniform mat4 model;

void main()
{
    vec4 pos = projection * mat4(mat3(view)) * model * vec4(position, 1.0f);
	gl_Position = pos.xyww;
    TexCoords = position;
}