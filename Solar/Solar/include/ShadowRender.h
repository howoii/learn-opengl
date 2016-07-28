#ifndef SHADOWRENDER_H
#define SHADOWRENDER_H

#include "Common.h"
#include "Shader.h"
#include "Texture2D.h"
#include "DirLight.h"

const GLuint SHADOW_WIDTH = 1024;
const GLuint SHADOW_HEIGHT = 1024;

class ShadowRender
{
public:
	Shader *DepthShader;
	Texture2D DepthMap;
	glm::mat4 LightSpaceMatrix;

	ShadowRender();
	ShadowRender(glm::vec3 lightDirection, GLfloat edge, GLfloat near, GLfloat far, Shader *shader);
	void BeginRender();
	void EndRender();
	void UpdateDirection(glm::vec3 direction);
	~ShadowRender();
private:
	GLuint DepthFBO;

	glm::vec3 LightDirection;
	GLfloat Edge;
	GLfloat NearPlane;
	GLfloat FarPlane;

	void setupFrameBuffer();
	void setupLightMatrix();
};


#endif // !SHADOWRENDER_H
