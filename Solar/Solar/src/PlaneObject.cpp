#include "PlaneObject.h"

PlaneObject::PlaneObject(){}

PlaneObject::~PlaneObject(){}

PlaneObject::PlaneObject(Mesh *mesh, Texture2D *texture, GLfloat side)
	:SObject(mesh, texture), Length(side), Width(side)
{
}

PlaneObject::PlaneObject(Mesh *mesh, Texture2D *texture, GLfloat length, GLfloat width)
	: SObject(mesh, texture), Length(length), Width(width)
{
}

void PlaneObject::Draw(Shader shader)
{

	this->prepareShader(shader);
	this->mesh->Draw(shader);
}

void PlaneObject::DrawInstance(Shader shader, std::vector<glm::mat4> modelMatrcies){
	this->prepareShader(shader);
	this->mesh->DrawInstanced(shader, modelMatrcies);
}

void PlaneObject::prepareShader(Shader shader)
{
	glm::mat4 model;
	model = glm::translate(model, this->Position);
	model = glm::scale(model, glm::vec3(this->Length, 1.0f, this->Width));
	shader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	this->texture->Bind();
	shader.SetInteger("diffuse", 0);
	shader.SetInteger("specular", 0);

	shader.SetVector3f("material.diffuse", glm::vec3(0.6));
	shader.SetVector3f("material.specular", glm::vec3(0.1f));
	shader.SetFloat("material.shininess", 1.0f);
}