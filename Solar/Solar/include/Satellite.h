#ifndef SATELLITE_H
#define SATELLITE_H

#include "Common.h"
#include "PlanetObject.h"

class Satellite : public PlanetObject
{
public:
	PlanetObject *HostPlanet;
	Satellite();
	Satellite(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> parameters);
	void SetHostPlanet(PlanetObject *host);
	void UpdatePosition(GLfloat time);
	void Draw(Shader shader);
	void DrawStar(Shader shader);
	~Satellite();

private:

};


#endif