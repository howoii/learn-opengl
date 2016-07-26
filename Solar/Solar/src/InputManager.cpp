#include "InputManager.h"

GLfloat InputManager::MouseX = 0.0f;
GLfloat InputManager::MouseY = 0.0f;
GLfloat InputManager::DeltaX = 0.0f;
GLfloat InputManager::DeltaY = 0.0f;
GLfloat InputManager::MouseScroll = 0.0f;

GLboolean InputManager::Keys[1024];
GLboolean InputManager::KeyBuffer[1024];
GLboolean InputManager::FirstMouse = true;

GLboolean InputManager::isKeyPressed(GLint key){
	return Keys[key];
}

GLboolean InputManager::isKeyProcessed(GLint key){
	return !KeyBuffer[key];
}


void InputManager::ProcessKeyInput(GLint key, GLint action){
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			Keys[key] = true;
			KeyBuffer[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Keys[key] = false;
			KeyBuffer[key] = false;
		}
	}
}
void InputManager::ProcessMouseInput(GLfloat x, GLfloat y){
	if (FirstMouse)
	{
		MouseX = x;
		MouseY = y;
		FirstMouse = false;
	}
	DeltaX = x - MouseX;
	DeltaY = MouseY - y;
	MouseX = x;
	MouseY = y;
}
void InputManager::PorcessScrollInput(GLfloat offset){
	MouseScroll = offset;
}

void InputManager::ProcessMouseEnter(){
	FirstMouse = true;
}

void InputManager::ClearMouseMovement(){
	DeltaX = 0.0f;
	DeltaY = 0.0f;
}

void InputManager::ClearMouseScroll(){
	MouseScroll = 0.0f;
}

void InputManager::ClearKeyBuffer(GLint key){
	KeyBuffer[key] = false;
}