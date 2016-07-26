#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H

#include "Common.h"
#include "SObject.h"

class PlaneObject : public SObject
{
public:
	GLfloat Length;
	GLfloat Width;

	PlaneObject();
	PlaneObject(Mesh *mesh, Texture2D *texture, GLfloat side = 1.0f);
	PlaneObject(Mesh *mesh, Texture2D *texture, GLfloat length, GLfloat width);
	void Draw(Shader shader);
	void DrawInstance(Shader shader, std::vector<glm::mat4> modelMatrcies);
	~PlaneObject();

private:
	void prepareShader(Shader shader);
};

#endif