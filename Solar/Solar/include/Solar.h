#ifndef SOLAR_H
#define SOLAR_H

#include "Common.h"

enum ViewMode
{
	SpaceMode,
	GroundMode
};

class Solar
{
public:
	ViewMode Vmode;
	GLuint Width, Height;

	Solar(GLuint width, GLuint height);
	~Solar();

	void Init();
	void ProcessInput(GLfloat deltaTime);
	void Update(GLfloat deltaTime);
	void Render();

private:
	void RenderSpace();
	void RenderGround();
	void OutPut(GLfloat dt);
};
#endif