#ifndef CUBE_H
#define CUBE_H

#include "OpenGLHeader.h"
#include "Shader.h"
#include <vector>

class Cube
{
public:
	Cube(GLfloat Scale = 1.0f);
	~Cube();
	void Draw(Shader shader);
	void Draw(Shader shader, glm::mat4 model);
	void translate(glm::vec3 Location);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat Rotate, glm::vec3 Dimension);
	glm::mat4 GetModelMatrix();
private:
	GLuint VAO, VBO;
	glm::vec3 Location;
	GLfloat Angle;
	glm::vec3 Axis;
	GLfloat Scale;
};

Cube::Cube(GLfloat Scale) :Location(glm::vec3(0.0f)), Angle(0.0f), Axis(glm::vec3(1.0f)), Scale(Scale)
{
	GLfloat vertices[] = {
		// Positions			// Normal      //TexCoords
		//Back Face
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

		 //Front Face
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

		//Left Face
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		//Right Face
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		//Bottom face
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

		 //Top Face
		 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	};
	GLint attrSize[] = { 3, 3, 2 };
	VBO = lglCreateBuffer(vertices, sizeof(vertices));
	VAO = lglCreateVertexArray(VBO, attrSize, 3);
}

void Cube::translate(glm::vec3 Location){
	this->Location = Location;
}

void Cube::translate(GLfloat x, GLfloat y, GLfloat z){
	this->Location = glm::vec3(x, y, z);
}

void Cube::rotate(GLfloat Angle, glm::vec3 Axis){
	this->Angle = Angle;
	this->Axis = Axis;
}

glm::mat4 Cube::GetModelMatrix(){
	glm::mat4 model;
	model = glm::translate(model, this->Location);
	model = glm::rotate(model, glm::radians(this->Angle), this->Axis);
	model = glm::scale(model, glm::vec3(this->Scale));

	return model;
}

void Cube::Draw(Shader shader){
	glm::mat4 model = GetModelMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Cube::Draw(Shader shader, glm::mat4 model){
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

Cube::~Cube()
{
}
#endif