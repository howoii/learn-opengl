#ifndef MESH_H
#define MESH_H

#include "Common.h"
#include "Shader.h"

#include <iostream>
#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 Postion;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	GLuint VAO;
	std::vector<Vertex> vertices;

	Mesh();
	Mesh(std::vector<Vertex> vertices);
	void Draw(Shader shader);
	void DrawInstanced(Shader shader, std::vector<glm::mat4> modelMatrices);
	static Mesh GetPlaneMesh();
	static Mesh GetCubeMesh();
	static Mesh GetSphereMesh(GLfloat radius = 1.0f);
	static glm::vec3 calcPos(GLfloat radius, GLfloat alpha, GLfloat beta);
	~Mesh();

private:
	void setupMesh();
};

#endif // !MESH_H
