#ifndef DIRECTLIGHT_H
#define DIRECTLIGHT_H

#include "LearnOpenGL/OpenGLHeader.h"
#include "LearnOpenGL/Shader.h"

class DirectLight
{
public:
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
		glm::vec3 specular);
	void Apply(Shader shader, const GLchar *name);
	~DirectLight();

private:

};

DirectLight::DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
	glm::vec3 specular) 
	:direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

void DirectLight::Apply(Shader shader, const GLchar *name){
	std::string structName(name);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".direction").c_str()), direction.x, direction.y, direction.z);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shader.Program, (structName + ".specular").c_str()), specular.x, specular.y, specular.z);
}

DirectLight::~DirectLight()
{
}
#endif // !DIRECTLIGHT_H
