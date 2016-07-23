#ifndef SOBJECT_H
#define SOBJECT_H

#include "Common.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "Shader.h"

class SObject
{
public:
	glm::vec3 Position;

	Mesh *mesh;
	Texture2D *texture;

	SObject(glm::vec3 pos, Mesh *mesh, Texture2D *texture);
	void MoveTo(glm::vec3 pos);
	~SObject();

private:

};

#endif // !SOBJECT_H
