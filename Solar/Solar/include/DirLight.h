#ifndef DIRLIGHT_H
#define  DIRLIGHT_H

#include "Common.h"
#include "Shader.h"

class DirLight
{
public:
	glm::vec3 Direction;
	glm::vec3 Color;
	GLfloat Brightness;
	GLfloat Ambient;

	DirLight();
	DirLight(glm::vec3 direction, GLfloat brightness, GLfloat ambient, glm::vec3 color = glm::vec3(1.0f));
	void SetUniformData(Shader shader, const GLchar *name, GLboolean useShader = GL_FALSE);
	~DirLight();

private:

};


#endif // !DIRLIGHT_H
