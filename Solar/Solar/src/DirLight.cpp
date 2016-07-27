#include "DirLight.h"

DirLight::DirLight()
{
}

DirLight::~DirLight()
{
}

DirLight::DirLight(glm::vec3 direction, GLfloat brightness, GLfloat ambient, glm::vec3 color /* = glm::vec3(1.0f) */)
	:Direction(direction), Brightness(brightness), Ambient(ambient), Color(color)
{
}

void DirLight::Update(glm::vec3 lightPos){
	this->Brightness = lightPos.y * SOLAR_BRIGHTNESS_LIGHT;
	this->Direction = -lightPos;
}

void DirLight::SetUniformData(Shader shader, const GLchar *name, GLboolean useShader /* = GL_FALSE */)
{
	if (useShader)
	{
		shader.Use();
	}
	std::string vName(name);
	shader.SetVector3f((vName + ".direction").c_str(), this->Direction);
	shader.SetVector3f((vName + ".color").c_str(), this->Color * glm::vec3(this->Brightness));
	shader.SetVector3f((vName + ".ambient").c_str(), glm::vec3(this->Ambient));
}