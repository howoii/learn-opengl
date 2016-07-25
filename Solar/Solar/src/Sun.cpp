#include "Sun.h"
#include "SolarMath.h"

Sun::Sun(){}

Sun::~Sun(){}

Sun::Sun(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> para)
	:SObject(mesh, texture)
{
	this->Radius = SolarMath::AU(para[0]);
	this->Brightness = para[1];
}

void Sun::Draw(Shader shader){
	glm::mat4 model;
	model = glm::translate(model, this->Position * glm::vec3(SOLAR_AXIS_SCALE));
	model = glm::scale(model, glm::vec3(this->Radius * SOLAR_SUN_SCALE * SOLAR_AXIS_SCALE));
	shader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	this->texture->Bind();
	shader.SetInteger("diffuse", 0);
	shader.SetInteger("specular", 0);

	glBindVertexArray(this->mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size());
	glBindVertexArray(0);
}