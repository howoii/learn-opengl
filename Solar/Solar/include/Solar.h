#ifndef SOLAR_H
#define SOLAR_H

#include "Common.h"

class Solar
{
public:
	GLuint Width, Height;

	Solar(GLuint width, GLuint height);
	~Solar();

	void Init();
	void ProcessInput(GLfloat deltaTime);
	void Update(GLfloat deltaTime);
	void Render();

private:

};
#endif