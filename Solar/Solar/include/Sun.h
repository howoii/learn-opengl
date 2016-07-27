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
	glm::vec3 ViewDirection;

	Sun();
	Sun(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> para);
	void UpdateViewDirecton(PlanetObject *earth, GLfloat longitude, GLfloat latitude);
	void Draw(Shader shader);
	~Sun();

private:

};
#endif // !SUN_H
