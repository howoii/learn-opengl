#include "SolarMath.h"
#include "math.h"

SolarMath::SolarMath()
{
}

SolarMath::~SolarMath()
{
}

GLfloat SolarMath::EccentricAnomaly(GLfloat M, GLfloat Eccentric, GLfloat error){
	GLfloat E0, E;
	E0 = M;
	E = E0 + (M + Eccentric * glm::sin(E0) - E0) / (1 - Eccentric * glm::cos(E0));
	GLint turns = 0;
	while (abs(E - E0) > error)
	{
		E0 = E;
		E = E0 + (M + Eccentric * glm::sin(E0) - E0) / (1 - Eccentric * glm::cos(E0));
		if (turns++ > 100) break;
	}
	return E;
}

void SolarMath::TrueAnomalyAndDistance(GLfloat E, GLfloat A, GLfloat Eccentric, GLfloat *r, GLfloat *theta){
	*r = A * (1 - Eccentric * glm::cos(E));
	GLfloat tanSemiTheta = sqrt((1 + Eccentric) / (1 - Eccentric)) * glm::tan(E / 2);
	*theta = 2 * glm::atan(tanSemiTheta);
}

GLfloat SolarMath::AU(GLfloat distance){
	return distance / SOLAR_AU;
}

GLfloat SolarMath::KM(GLfloat distance){
	return distance * SOLAR_AU;
}
