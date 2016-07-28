#include "PlanetObject.h"
#include "SolarMath.h"
#include "SolarTime.h"

PlanetObject::PlanetObject(){}

PlanetObject::PlanetObject(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> paramenters)
	:SObject(mesh, texture)
{
	if (paramenters.size() != 11)
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
	this->Reflect = paramenters[10];
	this->Position = this->CalcPostion(0.0f);
	this->InitPos = this->Position;
	this->UpdateRotation(0.0f);
}

glm::vec3 PlanetObject::CalcPostion(GLfloat time){
	GLfloat tSeconds = this->T * DAY_HOURS * HOUR_SECONDS;
	time -= int(time / tSeconds) * tSeconds;
	GLfloat meanAnomaly = glm::radians(time / tSeconds * 360.0 + this->M);
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
	posX = r * (cosOmegaTheta*cosBigOmega - sinOmegaTheta*sinBigOmega*cosI);
	posZ = -r * (cosOmegaTheta*sinBigOmega + sinOmegaTheta*cosBigOmega*cosI);
	posY = r * sinOmegaTheta*sinI;

	return glm::vec3(posX, posY, posZ);
}

void  PlanetObject::UpdatePosition(GLfloat time){
	this->Position = this->CalcPostion(time);
}

void PlanetObject::UpdateRotation(GLfloat time){
	GLfloat dSeconds = this->D * DAY_HOURS * HOUR_SECONDS;
	time -= int(time / dSeconds) * dSeconds;
	this->Rotation = 360.0f * (time / dSeconds);
}

void PlanetObject::UpdateViewDirecton(PlanetObject *earth, GLfloat longitude, GLfloat latitude){
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 viewUp;
	viewUp.y = glm::sin(glm::radians(latitude));
	viewUp.x = glm::cos(glm::radians(latitude));
	viewUp.z = 0;

	glm::mat4 rotateMatrix;
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(earth->O), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(earth->Rotation + longitude),
		glm::vec3(0.0f, 1.0f, 0.0f));

	worldUp = glm::mat3(rotateMatrix) * worldUp;
	viewUp = glm::mat3(rotateMatrix) * viewUp;
	glm::vec3 viewRight = glm::cross(worldUp, viewUp);
	glm::vec3 viewFront = glm::cross(viewUp, viewRight);

	glm::mat4 viewMatrix = glm::lookAt(earth->Position, earth->Position + viewFront, viewUp);
	glm::vec3 viewPos = glm::vec3(viewMatrix * glm::vec4(this->Position, 1.0f));

	this->ViewDirection = glm::normalize(viewPos);
}

void PlanetObject::UpdateViewSize(PlanetObject *earth){
	GLfloat distance = glm::distance(earth->Position, this->Position);
	if (distance != 0)
	{
		this->Distance = distance;
		this->ViewSize = (this->R / distance) * SOLAR_PLANET_VIEW_SCALE;
	}
}

void PlanetObject::Draw(Shader shader){
	glm::mat4 model;
	model = glm::translate(model, this->Position * glm::vec3(SOLAR_AXIS_SCALE));
	model = glm::rotate(model, glm::radians(this->O), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(this->Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(this->R * SOLAR_PLANET_SCALE * SOLAR_AXIS_SCALE));
	shader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	this->texture->Bind();
	shader.SetInteger("diffuse", 0);
	shader.SetInteger("specular", 0);

	shader.SetVector3f("material.diffuse", glm::vec3(this->Reflect));
	shader.SetVector3f("material.specular", glm::vec3(0.2f));
	shader.SetFloat("material.shininess", 32.0f);

	this->mesh->Draw(shader);
}

void PlanetObject::DrawStar(Shader shader){
	if (this->ViewDirection.y < -0.2) return;
	GLfloat starDistance = this->Distance + 50.0f;
	glm::vec3 starPos = this->ViewDirection * glm::vec3(starDistance);
	GLfloat size = starDistance * this->ViewSize;

	glm::mat4 model1;
	model1 = glm::translate(model1, starPos);
	model1 = glm::rotate(model1, glm::radians(this->O), glm::vec3(1.0f, 0.0f, 0.0f));
	model1 = glm::scale(model1, glm::vec3(size));
	shader.SetMatrix4("model1", model1);

	glm::mat4 model2;
	model2 = glm::translate(model2, this->Position);
	model2 = glm::rotate(model2, glm::radians(this->O), glm::vec3(1.0f, 0.0f, 0.0f));
	model2 = glm::scale(model2, glm::vec3(size));
	shader.SetMatrix4("model2", model2);

	glActiveTexture(GL_TEXTURE0);
	this->texture->Bind();
	shader.SetInteger("diffuse", 0);
	shader.SetInteger("specular", 0);

	shader.SetVector3f("material.diffuse", glm::vec3(this->Reflect));
	shader.SetVector3f("material.specular", glm::vec3(0.2f));
	shader.SetFloat("material.shininess", 32.0f);

	glDepthMask(GL_FALSE);
	this->mesh->Draw(shader);
	glDepthMask(GL_TRUE);
}


PlanetObject::~PlanetObject()
{
}