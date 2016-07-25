#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Shader.h"

const GLfloat SPEED = 50.0f;
const GLfloat SENSITIVITY = 0.02f;
const GLfloat ZOOM = 45.0f;

enum Camera_Move {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;

	GLfloat Pitch, Yaw;
	GLfloat MoveSpeed;
	GLfloat MouseSensivity;
	GLfloat Zoom;
	GLfloat Aspect;

	Camera(GLfloat aspect, GLfloat pitch = 0.0f, GLfloat yaw = 0.0f,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f));

	void Reset();
	void Reverse();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void BindUniformBuffer(GLuint bindPoint);
	void ProcessKeyBoard(Camera_Move direct, GLfloat deltatime);
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = GL_TRUE);
	void ProcessMouseScroll(GLfloat offset);

	~Camera();

private:
	GLuint UBO;
	GLboolean UboCreated;
	void updateCameraVectors();
	void updateUniformBuffer();
};

#endif