#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Common.h"

class InputManager
{
public:
	static GLfloat MouseX;
	static GLfloat MouseY;
	static GLfloat DeltaX;
	static GLfloat DeltaY;
	static GLfloat MouseScroll;
	static GLboolean isKeyPressed(GLint key);
	static void ProcessKeyInput(GLint key, GLint action);
	static void ProcessMouseInput(GLfloat x, GLfloat y);
	static void PorcessScrollInput(GLfloat offset);
	static void ProcessMouseEnter();
	static void ClearMouseMovement();
	static void ClearMouseScroll();
private:
	static GLboolean Keys[1024];
	static GLboolean FirstMouse;
	InputManager();
};

#endif // !INPUTMANAGER_H
