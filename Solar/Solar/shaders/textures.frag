#version 330 core
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	vec2 TexCoord2 = TexCoord * 2;
	color = vec4(0.2, 0.3, 0.8, 1.0);
}