#include "Satellite.h"


Satellite::Satellite()
{
}

Satellite::~Satellite()
{
}

Satellite::Satellite(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> parameters)
	:PlanetObject(mesh, texture, parameters)
{
}

void Satellite::SetHostPlanet(PlanetObject *host)
{
	this->HostPlanet = host;
}

void Satellite::UpdatePosition(GLfloat time)
{
	glm::vec3 relativePos = this->CalcPostion(time);
	this->Position = this->HostPlanet->Position + relativePos;
}

void Satellite::Draw(Shader shader)
{
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

void Satellite::DrawStar(Shader shader)
{
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
	model2 = glm::translate(model2, this->Position * glm::vec3(10.0f));
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
