#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "LearnOpenGL/OpenGLHeader.h"
#include "LearnOpenGL/Shader.h"

#define POINT_LIGHT7	glm::vec3(1.0f, 0.7f, 1.0f)
#define POINT_LIGHT100	glm::vec3(1.0f, 0.045, 0.0075)

const std::vector<glm::vec2> AttenuationFrac = {
	glm::vec2(0.7f, 1.8f),

};

class PointLight
{
public:
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, 
		glm::vec3 specular, glm::vec3 fraction);
	void Apply(Shader shader, const GLchar *name);
	~PointLight();

private:

};

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient,
	glm::vec3 diffuse, glm::vec3 specular, glm::vec3 fraction)
	:position(position), ambient(ambient), diffuse(diffuse), specular(specular)
{
	this->constant = fraction.x;
	this->linear = fraction.y;
	this->quadratic = fraction.z;
}

void PointLight::Apply(Shader shader, const GLchar *name){
	std::string structName(name);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".specular").c_str()), specular.x, specular.y, specular.z);
	glUniform1f(glGetUniformLocation(shader.Program, (structName + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shader.Program, (structName + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shader.Program, (structName + ".quadratic").c_str()), quadratic);
}

PointLight::~PointLight()
{
}

#endif