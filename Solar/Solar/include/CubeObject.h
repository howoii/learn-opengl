#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include "Common.h"
#include "SObject.h"

class CubeObject : public SObject
{
public:
	GLfloat Length;
	GLfloat Width;
	GLfloat Height;

	CubeObject();
	CubeObject(Mesh *mesh, Texture2D *texture, glm::vec3 shape = glm::vec3(1.0f));
	void Reshape(glm::vec3 shape);
	void Draw(Shader shader);
	~CubeObject();

private:
};

#endif