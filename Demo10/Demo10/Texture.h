#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGLHeader.h"
#include "Shader.h"

#include <iostream>
#include <vector>

class Texture
{
public:
	Texture(const GLchar *texPath);
	Texture(std::vector<const GLchar *> faces);
	void Active(GLuint texIndex);
	void Apply(Shader shader, const GLchar *samplerName);
	~Texture();

private:
	GLuint TexID;
	GLint TexIndex;
	GLenum TexTarget;
};

Texture::Texture(const GLchar *texPath)
	:TexIndex(-1)
{
	TexTarget = GL_TEXTURE_2D;
	TexID = lglLoadTexture2D(texPath);
}

Texture::Texture(std::vector<const GLchar *> faces){
	TexTarget = GL_TEXTURE_CUBE_MAP;
	TexID = lglLoadTextureCube(faces);
}

void Texture::Active(GLuint texIndex){
	if (texIndex < 0 || texIndex > 16)
	{
		std::cout << "ERROR::TEXTURE::ACTIVE\nIndex should between 0 and 15" << std::endl;
		return;
	}
	glActiveTexture(GL_TEXTURE0 + texIndex);
	glBindTexture(TexTarget, TexID);
	this->TexIndex = texIndex;
}

void Texture::Apply(Shader shader, const GLchar *samplerName){
	if (TexIndex < 0)
	{
		std::cout << "ERROR::TEXTURE::Apply\n Texture should be active before apply" << std::endl;
	}
	glUniform1i(glGetUniformLocation(shader.Program, samplerName), TexIndex);
}

Texture::~Texture()
{
}

#endif // !TEXTURE_H
