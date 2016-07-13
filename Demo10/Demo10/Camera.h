#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

const GLfloat SPEED = 2.0f;
const GLfloat SENSITIVITY = 0.25f;
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

	Camera(GLfloat pitch, GLfloat yaw, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f)):
		Pitch(pitch), Yaw(yaw), MoveSpeed(SPEED), MouseSensivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = worldup;
		this->updateCameraVectors();
	};

	glm::mat4 GetViewMatrix(){
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Attach(Shader shader, GLfloat aspect){
		glm::mat4 view = GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::perspective(glm::radians(ZOOM), aspect, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
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
		this->updateCameraVectors();
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
	}

	~Camera();

private:
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