#include "City.h"

City::City()
{
}

City::~City()
{
}

City::City(Mesh *mesh, Texture2D *texture, GLfloat size, GLfloat density, glm::vec2 center)
	:Building(mesh, texture), Size(size), Center(center)
{
	if (density < 0.0f || density > 1.0f){
		std::cout << "ERROR::CITY::CONSTRUCTOR Density out of range(0.0~1.0)" << std::endl;
		this->Density = 1.0f;
	}
	else
	{
		this->Density = density;
	}
	this->BlockSize = 10.0f;
	this->generate();
}

void City::generate(){
	this->Positions.clear();
	this->Shapes.clear();

	GLint blockNum = GLint(this->Size / this->BlockSize);
	GLfloat sideMin = 0.3;
	GLfloat sideMax = 0.8;
	GLfloat heightMin = 1.0;
	GLfloat heightMax = 10.0;

	srand(glfwGetTime());
	for (GLint x = 0; x < blockNum; x++)
	{
		GLfloat posX = -(blockNum - 1) / 2.0 + x;
		for (GLint z = 0; z < blockNum; z++)
		{
			GLfloat posZ = -(blockNum - 1) / 2.0 + z;
			GLint random = rand();
			if ((random % 100) / 100.0f < this->Density)
			{
				GLfloat ranX = (rand() % 100) / 100.0f * (sideMax - sideMin) + sideMin;
				GLfloat ranY = (rand() % 100) / 100.0f * (heightMax - heightMin) + heightMin;
				GLfloat ranZ = (rand() % 100) / 100.0f * (sideMax - sideMin) + sideMin;
				glm::vec2 position = glm::vec2(posX, posZ) * glm::vec2(this->BlockSize) + this->Center;
				glm::vec3 shape = glm::vec3(ranX, ranY, ranZ) * glm::vec3(this->BlockSize);
				this->Positions.push_back(position);
				this->Shapes.push_back(shape);
			}
		}
	}
}

void City::Draw(Shader shader)
{
	for (GLint i = 0; i < this->Positions.size(); i++)
	{
		glm::vec2 position = this->Positions[i];
		glm::vec3 shape = this->Shapes[i];
		this->Building.MoveTo(glm::vec3(position.x, shape.y / 2.0, position.y));
		this->Building.Reshape(shape);
		this->Building.Draw(shader);
	}
}