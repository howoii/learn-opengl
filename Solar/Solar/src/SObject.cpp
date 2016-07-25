#include "SObject.h"

SObject::SObject(){
	this->Position = glm::vec3(0.0f);
}

SObject::SObject(Mesh *mesh, Texture2D *texture){
	this->Position = glm::vec3(0.0f);
	this->mesh = mesh;
	this->texture = texture;
}

void SObject::MoveTo(glm::vec3 pos){
	this->Position = pos;
}

SObject::~SObject(){}