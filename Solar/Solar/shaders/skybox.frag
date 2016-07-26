#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform float brightness;
uniform samplerCube skybox;

void main()
{
	vec3 texColor = vec3(texture(skybox, TexCoords));
	color = vec4(texColor * brightness, 1.0f);
}