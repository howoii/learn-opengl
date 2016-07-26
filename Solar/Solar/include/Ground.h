#ifndef GROUND_H
#define GROUND_H

#include "Common.h"
#include "PlaneObject.h"
#include <vector>

class Ground
{
public:
	GLfloat Length, Width;
	PlaneObject Terrian;
	Ground();
	Ground(Mesh *mesh, Texture2D *texture, GLfloat length, GLfloat width);
	void Draw(Shader shder);
	~Ground();

private:
	std::vector<glm::mat4> modelMatrcies;
	void generateModelMatrcies();
};


#endif // !FROUND_H
