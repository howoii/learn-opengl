#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include "Common.h"
#include "Shader.h"
#include <vector>

class TextureCube
{
public:
	GLuint ID;

	GLuint Internal_Format;
	GLuint Image_Format;

	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Wrap_R;
	GLuint Filter_Min;
	GLuint Filter_Mag;

	TextureCube();
	void Generate(std::vector<std::string> faces);
	void Bind() const;
	~TextureCube();

private:

};

#endif