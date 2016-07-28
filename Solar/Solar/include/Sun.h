#ifndef SUN_H
#define SUN_H

#include "Common.h"
#include "SObject.h"
#include "PlanetObject.h"

class Sun: public SObject
{
public:
	GLfloat Radius;
	GLfloat Brightness;

	GLfloat ViewSize;
	glm::vec3 ViewDirection;
	GLfloat Distance;

	Sun();
	Sun(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> para);
	void UpdateViewDirecton(PlanetObject *earth, GLfloat longitude, GLfloat latitude);
	void UpdateViewSize(PlanetObject *earth);
	void Draw(Shader shader);
	void DrawStar(Shader shader);
	~Sun();

private:

};
#endif // !SUN_H
