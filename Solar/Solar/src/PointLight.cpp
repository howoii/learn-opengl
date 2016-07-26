#include "PointLight.h"

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

PointLight::PointLight(glm::vec3 position, GLfloat brightness, GLfloat ambient, glm::vec3 color)
	:Position(position), Brightness(brightness), Ambient(ambient), Color(color)
{

}
void PointLight::SetUniformData(Shader shader, const GLchar *name, GLboolean useShader){
	if (useShader)
	{
		shader.Use();
	}
	std::string vName(name);
	shader.SetVector3f((vName + ".position").c_str(), this->Position);
	shader.SetVector3f((vName + ".color").c_str(), this->Color * glm::vec3(this->Brightness));
	shader.SetVector3f((vName + ".ambient").c_str(), glm::vec3(this->Ambient));
}