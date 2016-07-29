#include "Ground.h"


Ground::Ground()
{
}

Ground::~Ground()
{
}

Ground::Ground(Mesh *mesh, Texture2D *texture, GLfloat length, GLfloat width)
	:Length(length), Width(width), BrickSize(1.0f)
{
	this->Terrian = PlaneObject(mesh, texture, this->BrickSize);
}

void Ground::SetBrickSize(GLfloat size){
	this->BrickSize = size;
	this->Terrian.Length = size;
	this->Terrian.Width = size;
}

void Ground::Draw(Shader shader)
{
	for (GLint x = 0; x < this->Length; x++)
	{
		GLfloat posX = -(this->Length - 1) / 2.0 + x;
		for (GLint z = 0; z < this->Width; z++)
		{
			GLfloat posZ = -(this->Width - 1) / 2.0 + z;
			this->Terrian.MoveTo(glm::vec3(posX, 0.0f, posZ) * glm::vec3(this->BrickSize));
			this->Terrian.Draw(shader);
		}
	}
}