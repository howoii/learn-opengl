#ifndef POINTLIGHTRENDERER_H
#define POINTLIGHTRENDERER_H

#include "Common.h"
#include "Shader.h"
#include "SObject.h"

class PointLightRenderer
{
public:
	Shader *shader;

	glm::vec3 Position;
	GLfloat Brightness;
	GLfloat Ambient;
	GLfloat Shininess;

	PointLightRenderer();
	PointLightRenderer(Shader *shader, glm::vec3 position, GLfloat ambient, GLfloat brightness);
	void Render(SObject *obj, GLfloat specular, glm::vec3 viewPos);
	~PointLightRenderer();

private:

};

#endif