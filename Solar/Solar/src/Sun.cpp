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

void Sun::UpdateViewDirecton(PlanetObject *earth, GLfloat longitude, GLfloat latitude){
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 viewUp;
	viewUp.y = glm::sin(glm::radians(latitude));
	viewUp.x = glm::cos(glm::radians(latitude));
	viewUp.z = 0;

	glm::mat4 rotateMatrix;
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(earth->O), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(earth->Rotation + longitude),
		glm::vec3(0.0f, 1.0f, 0.0f));

	worldUp = glm::mat3(rotateMatrix) * worldUp;
	viewUp = glm::mat3(rotateMatrix) * viewUp;
	glm::vec3 viewRight = glm::cross(worldUp, viewUp);
	glm::vec3 viewFront = glm::cross(viewUp, viewRight);

	glm::mat4 viewMatrix = glm::lookAt(earth->Position, earth->Position + viewFront, viewUp);
	glm::vec3 viewPos = glm::vec3(viewMatrix * glm::vec4(this->Position, 1.0f));

	this->ViewDirection = glm::normalize(viewPos);
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

	shader.SetFloat("brightness", this->Brightness);

	this->mesh->Draw(shader);
}