#ifndef CAMERA_H
#define CAMERA_H

#include "OpenGLHeader.h"
#include "Shader.h"

#include <iostream>

const GLfloat SPEED = 2.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;
const GLfloat ASPECT = GLfloat(WINDOW_WIDTH) / WINDOW_HEIGHT;

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

	Camera(GLfloat pitch = 0.0f, GLfloat yaw = 0.0f, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f)):
		Pitch(pitch), Yaw(yaw), MoveSpeed(SPEED), MouseSensivity(SENSITIVITY), Zoom(ZOOM), Aspect(ASPECT)
	{
		this->Position = position;
		this->WorldUp = worldup;
		this->UboCreated = GL_FALSE;
		this->updateCameraVectors();
	};

	void Reset(){
		this->Position = glm::vec3(0.0f);
		this->Pitch = 0.0f;
		this->Yaw = 0.0f;
		this->Zoom = ZOOM;
		this->updateCameraVectors();
		this->updateUniformBuffer();
	}

	void Reverse(){
		Pitch = -Pitch;
		Yaw += 180.0f;
		this->updateCameraVectors();
		this->updateUniformBuffer();
	}

	glm::mat4 GetViewMatrix(){
		return glm::lookAt(Position, Position + Front, Up);
	}

	glm::mat4 GetProjectionMatrix(){
		return glm::perspective(glm::radians(Zoom), Aspect, 0.1f, 100.0f);
	}

	void updateUniformBuffer(){
		glm::mat4 view = GetViewMatrix();
		glm::mat4 projection = GetProjectionMatrix();
		lglUniformBufferData(UBO, 0, sizeof(glm::mat4), glm::value_ptr(view));
		lglUniformBufferData(UBO, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	}

	void BindUniformBuffer(GLuint bindPoint){
		if (!UboCreated)
		{
			this->UBO = lglCreateUniformBuffer(2 * sizeof(glm::mat4));
			UboCreated = GL_TRUE;
		}
		glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, this->UBO, 0, 2 * sizeof(glm::mat4));
	}

	void ProcessKeyBoard(Camera_Move direct, GLfloat deltatime){
		GLfloat velocity = MoveSpeed * deltatime;
		if (direct == FORWARD)
		{
			Position += velocity * Front;
		}
		if (direct == BACKWARD)
		{
			Position -= velocity * Front;
		}
		if (direct == RIGHT)
		{
			Position += velocity * Right;
		}
		if (direct == LEFT)
		{
			Position -= velocity * Right;
		}
		this->updateUniformBuffer();
	}

	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = GL_TRUE){
		Pitch += MouseSensivity * yOffset;
		Yaw += MouseSensivity * xOffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
			{
				Pitch = 89.0f;
			}
			if (Pitch < -89.0f)
			{
				Pitch = -89.0f;
			}
		}
		this->updateCameraVectors();
		this->updateUniformBuffer();
	}

	void ProcessMouseScroll(GLfloat offset){
		Zoom -= offset;
		if (Zoom > 75.0f)
		{
			Zoom = 75.0f;
		}
		if (Zoom < 1.0f)
		{
			Zoom = 1.0f;
		}
		this->updateUniformBuffer();
	}

	~Camera();

private:
	GLuint UBO;
	GLboolean UboCreated;

	void updateCameraVectors(){
		Front.y = sin(glm::radians(Pitch));
		Front.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front.z = -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		Front = glm::normalize(Front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

Camera::~Camera()
{
}

#endif