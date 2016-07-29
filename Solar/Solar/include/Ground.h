#ifndef GROUND_H
#define GROUND_H

#include "Common.h"
#include "PlaneObject.h"
#include <vector>

class Ground
{
public:
	GLfloat Length, Width;
	GLfloat BrickSize;
	Ground();
	Ground(Mesh *mesh, Texture2D *texture, GLfloat length, GLfloat width);
	void SetBrickSize(GLfloat size);
	void Draw(Shader shader);
	~Ground();

private:
	PlaneObject Terrian;
};


#endif // !FROUND_H
