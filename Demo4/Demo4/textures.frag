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
	color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord2), 0.3);
}