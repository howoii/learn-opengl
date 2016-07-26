#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

SkyBox::SkyBox(Mesh *mesh, TextureCube *cubeMap)
	:mesh(mesh), cubeMap(cubeMap), Size(1.0f), Position(glm::vec3(0.0f)), Brightness(1.0f)
{

}

void SkyBox::UpdatePosition(glm::vec3 position){
	this->Position = position;
}

void SkyBox::UpdateBrightness(GLfloat brightness){
	this->Brightness = brightness;
}

void SkyBox::Draw(Shader shader){
	glm::mat4 model;
	model = glm::translate(model, this->Position);
	model = glm::scale(model, glm::vec3(this->Size));
	shader.SetMatrix4("model", model);

	shader.SetFloat("brightness", this->Brightness);

	glActiveTexture(GL_TEXTURE0);
	this->cubeMap->Bind();
	shader.SetInteger("skybox", 0);

	this->mesh->Draw(shader);
}