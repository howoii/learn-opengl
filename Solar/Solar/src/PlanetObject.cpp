#include "PlanetObject.h"
#include "SolarMath.h"

PlanetObject::PlanetObject(glm::vec3 initPos, Mesh *mesh, Texture2D *texture)
	:SObject(initPos, mesh, texture)
{
}

void  PlanetObject::UpdatePosition(GLfloat time){
	time -= int(time / this->T) * this->T;
	GLfloat meanAnomaly = glm::radians(time / this->T * 360.0 + this->M);
	GLfloat r, theta, E;
	E = SolarMath::EccentricAnomaly(meanAnomaly, this->¦Å);
	SolarMath::TrueAnomalyAndDistance(E, this->A, this->¦Å, &r, &theta);

	GLfloat posX, posY, posZ;
	GLfloat sinOmegaTheta = glm::sin(glm::radians(this->¦Ø) + theta);
	GLfloat cosOmegaTheta = glm::cos(glm::radians(this->¦Ø) + theta);
	GLfloat sinI = glm::sin(glm::radians(this->I));
	GLfloat cosI = glm::cos(glm::radians(this->I));
	GLfloat sinBigOmega = glm::sin(glm::radians(this->¦¸));
	GLfloat cosBigOmega = glm::cos(glm::radians(this->¦¸));
	posX =  r * (cosOmegaTheta*cosBigOmega - sinOmegaTheta*sinBigOmega*cosI);
	posZ = -r * (cosOmegaTheta*sinBigOmega + sinOmegaTheta*cosBigOmega*cosI);
	posY =  r * sinOmegaTheta*sinI;

	this->Position = glm::vec3(posX, posY, posZ);
}

PlanetObject::~PlanetObject()
{
}