#include "Camera.h"

Camera::Camera(GLfloat aspect, GLfloat pitch, GLfloat yaw, glm::vec3 position, glm::vec3 worldup) :
Pitch(pitch), Yaw(yaw), MoveSpeed(SPEED), MouseSensivity(SENSITIVITY), Zoom(ZOOM), Aspect(aspect)
{
	this->Position = position;
	this->WorldUp = worldup;
	this->UboCreated = GL_FALSE;
	this->updateCameraVectors();
};

void Camera::Reset(){
	this->Position = glm::vec3(0.0f);
	this->Pitch = 0.0f;
	this->Yaw = 0.0f;
	this->Zoom = ZOOM;
	this->updateCameraVectors();
	this->updateUniformBuffer();
}

void Camera::Reverse(){
	Pitch = -Pitch;
	Yaw += 180.0f;
	this->updateCameraVectors();
	this->updateUniformBuffer();
}

glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(){
	return glm::perspective(glm::radians(Zoom), Aspect, 0.1f, 1000.0f);
}

void Camera::updateUniformBuffer(){
	glm::mat4 view = GetViewMatrix();
	glm::mat4 projection = GetProjectionMatrix();

	glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::BindUniformBuffer(GLuint bindPoint){
	if (!UboCreated)
	{
		glGenBuffers(1, &this->UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		UboCreated = GL_TRUE;
	}
	glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, this->UBO, 0, 2 * sizeof(glm::mat4));
	this->updateUniformBuffer();
}

void Camera::ProcessKeyBoard(Camera_Move direct, GLfloat deltatime){
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

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch){
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

void Camera::ProcessMouseScroll(GLfloat offset){
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

void Camera::updateCameraVectors(){
	Front.y = sin(glm::radians(Pitch));
	Front.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front.z = -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	Front = glm::normalize(Front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

Camera::~Camera()
{
}