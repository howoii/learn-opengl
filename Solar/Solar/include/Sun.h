#ifndef SUN_H
#define SUN_H

#include "Common.h"
#include "SObject.h"

class Sun: public SObject
{
public:
	GLfloat Radius;

	Sun();
	Sun(Mesh *mesh, Texture2D *texture, GLfloat radius);
	void Draw(Shader shader);
	~Sun();

private:

};
#endif // !SUN_H
