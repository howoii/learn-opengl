#include "Ground.h"


Ground::Ground()
{
}

Ground::~Ground()
{
}

Ground::Ground(Mesh *mesh, Texture2D *texture, GLfloat length, GLfloat width)
	:Length(length), Width(width)
{
	this->Terrian = PlaneObject(mesh, texture, 30.0f);
	this->generateModelMatrcies();
}

void Ground::Draw(Shader shder)
{
	this->Terrian.DrawInstance(shder, modelMatrcies);
}

void Ground::generateModelMatrcies()
{
	for (GLint x = 0; x < this->Length; x ++)
	{
		GLfloat posX = -(this->Length - 1) / 2.0 + x;
		for (GLint z = 0; z < this->Width; z++)
		{
			GLfloat posZ = -(this->Width - 1) / 2.0 + z;
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(posX, 0.0f, posZ) * glm::vec3(30.0f));
			this->modelMatrcies.push_back(model);
		}
	}
}