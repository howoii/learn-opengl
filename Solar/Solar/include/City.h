#ifndef CITY_H
#define CITY_H

#include "Common.h"
#include "CubeObject.h"
#include <vector>

class City
{
public:
	City();
	City(Mesh *mesh, Texture2D *texture, GLfloat size, GLfloat density, glm::vec2 center = glm::vec2(0.0f));
	void Draw(Shader shader);
	~City();
private:
	CubeObject Building;
	GLfloat Size;
	GLfloat Density;
	GLfloat BlockSize;
	glm::vec2 Center;

	std::vector<glm::vec2> Positions;
	std::vector<glm::vec3> Shapes;

	void generate();
};

#endif // !CITY_H
