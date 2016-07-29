#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;
	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(position, 1.0));
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}