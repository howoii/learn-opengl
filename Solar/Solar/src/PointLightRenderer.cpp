#include "PointLightRenderer.h"

PointLightRenderer::PointLightRenderer()
{
}

PointLightRenderer::~PointLightRenderer()
{
}

PointLightRenderer::PointLightRenderer(Shader *shader, glm::vec3 position,
	GLfloat ambient, GLfloat brightness)
	:shader(shader), Position(position), Ambient(ambient), Brightness(brightness)
{
	this->Shininess = 32.0f;
}

void PointLightRenderer::Render(SObject *obj, GLfloat specular, glm::vec3 viewPos){
	shader->Use();
	GLfloat diffuse = 0.8f;
	shader->SetVector3f("pointLight.position", this->Position);
	shader->SetVector3f("pointLight.ambient", glm::vec3(this->Ambient));
	shader->SetVector3f("pointLight.diffuse", glm::vec3(diffuse));
	shader->SetVector3f("pointLight.specular", glm::vec3(specular));
	shader->SetVector3f("viewPos", viewPos);
	shader->SetFloat("shininess", this->Shininess);
	shader->SetFloat("brightness", this->Brightness);

	obj->Draw(*shader);
}