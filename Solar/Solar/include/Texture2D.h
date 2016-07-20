#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Common.h"

class Texture2D
{
public:
	GLuint ID;
	GLuint width, height;

	GLuint Internal_Format;
	GLuint Image_Format;

	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Mag;

	Texture2D();
	~Texture2D();

	void Generate(GLuint width, GLuint height, unsigned char *data);
	void Bind() const;

private:

};


#endif // !TEXTURE_H
