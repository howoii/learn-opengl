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

uniform mat4 model1;
uniform mat4 model2;

void main()
{
    vec4 pos = projection * mat4(mat3(view)) * model1 * vec4(position, 1.0);
	gl_Position = pos.xyww;
    TexCoords = texCoords;
	Normal = mat3(transpose(inverse(model2))) * normal;
	FragPos = vec3(model2 * vec4(position, 1.0));
}