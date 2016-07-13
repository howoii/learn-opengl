#ifndef MESH_H
#define MESH_H

#include "OpenGLHeader.h"
#include "Shader.h"
#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();
	void Draw(Shader shader);
	void translate(glm::vec3 Location);
	void rotate(GLfloat Rotate, glm::vec3 Dimension);
	void scale(GLfloat Scale);
private:

};

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

#endif