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
uniform mat4 lightSpaceMatrix;


uniform sampler2D NormalTex;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	vec3 norm = normalize(texture(NormalTex, texCoords).rgb * 2.0 - 1.0);
	Normal = mat3(transpose(inverse(mat3(model)))) * norm;
	FragPos = vec3(model * vec4(position, 1.0));
	TexCoords = texCoords;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}