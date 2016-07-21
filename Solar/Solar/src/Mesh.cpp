#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices){
	this->vertices = vertices;
	this->setupMesh();
}

Mesh::~Mesh(){}

Mesh Mesh::GetPlaneMesh(){
	std::vector<Vertex> vertices;
	Vertex vertex;
	GLfloat points[] = {
		// Positions			// Normal      //TexCoords
		-0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};
	for (GLuint i = 0; i < 6; i++)
	{
		vertex.Postion = glm::vec3(points[8 * i], points[8 * i + 1], points[8 * i + 2]);
		vertex.Normal = glm::vec3(points[8 * i + 3], points[8 * i + 4], points[8 * i + 5]);
		vertex.TexCoords = glm::vec2(points[8 * i + 6], points[8 * i + 7]);
		vertices.push_back(vertex);
	}
	return Mesh(vertices);
}

Mesh Mesh::GetCubeMesh(){
	std::vector<Vertex> vertices;
	Vertex vertex;
	GLfloat points[] = {
		//Back Face
		 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

		//Front Face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		//Left Face
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		//Right Face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		//Bottom face
		 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

		//Top Face
		 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};
	for (GLuint i = 0; i < 36; i++)
	{
		vertex.Postion = glm::vec3(points[8 * i], points[8 * i + 1], points[8 * i + 2]);
		vertex.Normal = glm::vec3(points[8 * i + 3], points[8 * i + 4], points[8 * i + 5]);
		vertex.TexCoords = glm::vec2(points[8 * i + 6], points[8 * i + 7]);
		vertices.push_back(vertex);
	}

	return Mesh(vertices);
}

Mesh Mesh::GetSphereMesh(GLfloat radius){
	GLint division = 12;

	std::vector<Vertex> vertices;
	Vertex vertex;
	glm::vec3 positions[4];
	glm::vec2 texCoords[4];
	GLuint indices[] = { 0, 3, 2, 2, 1, 0 };

	GLfloat alpha;
	GLfloat beta;

	GLfloat angleStrip = 360.0f / division;

	for (GLuint y = 0; y < division / 2; y++){
		beta = (y - division / 4) * angleStrip;
		for (GLuint x = 0; x < division; x++)
		{
			alpha = x * angleStrip;
			//bottom right
			positions[0] = calcPos(radius, alpha, beta);
			texCoords[0] = glm::vec2(GLfloat(x) / division, GLfloat(y) * 2.0 / division);
			//bottom left
			positions[1] = calcPos(radius, alpha + angleStrip, beta);
			texCoords[1] = glm::vec2(GLfloat(x + 1) / division, GLfloat(y) * 2.0 / division);
			//top left
			positions[2] = calcPos(radius, alpha + angleStrip, beta + angleStrip);
			texCoords[2] = glm::vec2(GLfloat(x + 1) / division, GLfloat(y + 1) * 2.0 / division);
			//top right
			positions[3] = calcPos(radius, alpha, beta + angleStrip);
			texCoords[3] = glm::vec2(GLfloat(x) / division, GLfloat(y + 1) * 2.0 / division);
			for (GLuint i = 0; i < 6; i++)
			{
				vertex.Postion = positions[indices[i]];
				vertex.Normal = vertex.Postion;
				vertex.TexCoords = texCoords[indices[i]];
			}
			vertices.push_back(vertex);
		}
	}
	return Mesh(vertices);
}

void Mesh::setupMesh(){
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

glm::vec3 Mesh::calcPos(GLfloat radius, GLfloat alpha, GLfloat beta){
	glm::vec3 position;
	GLfloat rAlpha = glm::radians(alpha);
	GLfloat rBeta = glm::radians(beta);
	position.y = radius * glm::sin(rBeta);
	position.z = radius	* glm::cos(rBeta) * glm::sin(rAlpha);
	position.x = radius	* glm::cos(rBeta) * glm::cos(rAlpha);

	return position;
}