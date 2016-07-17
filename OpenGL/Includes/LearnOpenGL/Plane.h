#ifndef PLANE_H
#define PLANE_H

#include "OpenGLHeader.h"
#include "Shader.h"
#include <vector>

class Plane
{
public:
	Plane(GLfloat ScaleX = 1.0f, GLfloat ScaleZ = 1.0f);
	~Plane();
	void Draw(Shader shader);
	void translate(glm::vec3 Location);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat Rotate, glm::vec3 Axis);

private:
	GLuint VAO, VBO;
	glm::vec3 Location;
	GLfloat Angle;
	glm::vec3 Axis;
	GLfloat ScaleX;
	GLfloat ScaleZ;
};

Plane::Plane(GLfloat ScaleX, GLfloat ScaleZ) 
	:ScaleX(ScaleX), ScaleZ(ScaleZ)
{
	GLfloat vertices[] = {
		// Positions			// Normal      //TexCoords
		-0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};
	GLint attrSize[] = { 3, 3, 2 };
	VBO = lglCreateBuffer(vertices, sizeof(vertices));
	VAO = lglCreateVertexArray(VBO, attrSize, 3);

	this->Location = glm::vec3(0.0f);
	this->Angle = 0.0f;
	this->Axis = glm::vec3(1.0f);
}

Plane::~Plane()
{
}

void Plane::translate(glm::vec3 Location){
	this->Location = Location;
}

void Plane::translate(GLfloat x, GLfloat y, GLfloat z){
	this->Location = glm::vec3(x, y, z);
}

void Plane::rotate(GLfloat Angle, glm::vec3 Axis){
	this->Angle = Angle;
	this->Axis = Axis;
}

void Plane::Draw(Shader shader){
	glm::mat4 model;
	model = glm::translate(model, this->Location);
	model = glm::rotate(model, glm::radians(this->Angle), this->Axis);
	model = glm::scale(model, glm::vec3(this->ScaleX, 1.0f, this->ScaleZ));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

#endif // !PLANE_H
