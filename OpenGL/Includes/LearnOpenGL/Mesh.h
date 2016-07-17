#ifndef MESH_H
#define MESH_H

#include "OpenGLHeader.h"
#include "Shader.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	void Draw(Shader shader);
	~Mesh();

private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader shader){
	GLuint diffuseNR = 1;
	GLuint specularNR = 1;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		std::stringstream ss;
		std::string number;
		std::string type= this->textures[i].type;

		if (type == "texture_diffuse")
		{
			ss << diffuseNR++;
		}else if (type == "texture_specular")
		{
			ss << specularNR++;
		}
		number = ss.str();
		glUniform1f(glGetUniformLocation(shader.Program, ("material." + type + number).c_str()), i);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh(){
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

#endif // !MESH_H
