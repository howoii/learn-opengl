#include "PlanetObject.h"
#include "SolarMath.h"

PlanetObject::PlanetObject(){}

PlanetObject::PlanetObject(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> paramenters)
	:SObject(mesh, texture)
{
	if (paramenters.size() != 10)
	{
		std::cout << "ERROR::PLANETOBJECT::CONSTRUCTOR Paramenter number error" << std::endl;
		return;
	}
	this->T = paramenters[0];
	this->A = paramenters[1];
	this->¦Å = paramenters[2];
	this->I = paramenters[3];
	this->¦Ø = paramenters[4];
	this->¦¸ = paramenters[5];
	this->M = paramenters[6];
	this->R = SolarMath::AU(paramenters[7]);
	this->D = paramenters[8];
	this->O = paramenters[9];
	this->UpdatePosition(0.0f);
	this->UpdateRotation(0.0f);
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

void PlanetObject::UpdateRotation(GLfloat time){
	time -= int(time / this->D) * this->D;
	this->Rotation = 360.0f * (time / this->D);
}

void PlanetObject::Draw(Shader shader){
	glm::mat4 model;
	model = glm::translate(model, this->Position * glm::vec3(SOLAR_AXIS_SCALE));
	model = glm::rotate(model, this->O, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, this->Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(this->R * SOLAR_PLANET_SCALE * SOLAR_AXIS_SCALE));
	shader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	this->texture->Bind();
	shader.SetInteger("diffuse", 0);
	shader.SetInteger("specular", 0);

	glBindVertexArray(this->mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size());
	glBindVertexArray(0);
}

PlanetObject::~PlanetObject()
{
}