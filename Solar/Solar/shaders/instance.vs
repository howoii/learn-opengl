#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 instanceMatrix;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;

void main()
{
    gl_Position = projection * view * instanceMatrix * model * vec4(position, 1.0f);
    TexCoords = texCoords;
	Normal = mat3(transpose(inverse(instanceMatrix * model))) * normal;
	FragPos = vec3(model * vec4(position, 1.0));
}