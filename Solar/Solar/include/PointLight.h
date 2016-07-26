#ifndef PointLight_H
#define PointLight_H

#include "Common.h"
#include "Shader.h"

class PointLight
{
public:
	glm::vec3 Position;
	glm::vec3 Color;
	GLfloat Brightness;
	GLfloat Ambient;

	PointLight();
	PointLight(glm::vec3 position, GLfloat brightness, GLfloat ambient, glm::vec3 color = glm::vec3(1.0f));
	void SetUniformData(Shader shader, const GLchar *name, GLboolean useShader = GL_FALSE);
	~PointLight();

private:

};


#endif // !PointLight_H
