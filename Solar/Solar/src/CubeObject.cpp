#include "CubeObject.h"

CubeObject::CubeObject()
{
}

CubeObject::~CubeObject()
{
}

CubeObject::CubeObject(Mesh *mesh, Texture2D *texture, glm::vec3 shape)
	:SObject(mesh, texture)
{
	this->Reshape(shape);
}

void CubeObject::Reshape(glm::vec3 shape){
	this->Length = shape.x;
	this->Height = shape.y;
	this->Width = shape.z;
}

void CubeObject::Draw(Shader shader){
	glm::mat4 model;
	model = glm::translate(model, this->Position);
	model = glm::scale(model, glm::vec3(this->Length, this->Height, this->Width));
	shader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	this->texture->Bind();
	shader.SetInteger("diffuse", 0);
	shader.SetInteger("specular", 0);

	shader.SetVector3f("material.diffuse", glm::vec3(0.8));
	shader.SetVector3f("material.specular", glm::vec3(0.4f));
	shader.SetFloat("material.shininess", 1.0f);

	this->mesh->Draw(shader);
}