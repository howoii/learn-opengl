#ifndef SOBJECT_H
#define SOBJECT_H

#include "Common.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "Shader.h"

class SObject
{
public:
	glm::vec3 position;
	glm::mat4 model;

	Mesh *mesh;
	Texture2D *texture;

	void MoveTo(glm::vec3 pos);
	void RotateTo(GLfloat angle, glm::vec3 dir);
	void Draw(Shader shader);
	SObject();
	~SObject();

private:

};

#endif // !SOBJECT_H
