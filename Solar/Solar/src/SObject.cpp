#include "SObject.h"

SObject::SObject(glm::vec3 pos, Mesh *mesh, Texture2D *texture){
	this->Position = pos;
	this->mesh = mesh;
	this->texture = texture;
}

void SObject::MoveTo(glm::vec3 pos){
	this->Position = pos;
}

SObject::~SObject(){}