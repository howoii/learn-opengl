#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "PlanetObject.h"
#include "SolarMath.h"


Camera camera(GLfloat(4)/3, 0.0, 0.0, glm::vec3(0.0f, 0.0f, 100.0f));
PlanetObject *planet;
PlanetObject *sun;

Solar::Solar(GLuint width, GLuint height)
	:Width(width), Height(height)
{

}

Solar::~Solar(){

}

void Solar::Init(){
	Shader ourShader = ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic");
	ourShader.SetUniformBlock("Camera", 0);
	ResourceManager::LoadTexture("textures/planet_textures/texture_earth_clouds.jpg", GL_FALSE, "earth");
	ResourceManager::LoadTexture("textures/planet_textures/texture_sun.jpg", GL_FALSE, "sun");
	ResourceManager::StoreMesh(Mesh::GetSphereMesh(), "sphere");

	Mesh *sphere = &ResourceManager::Meshes["sphere"];
	Texture2D *saturnTexture = &ResourceManager::Textures["earth"];
	Texture2D *sunTexture = &ResourceManager::Textures["sun"];

	planet = new PlanetObject(glm::vec3(0.0f), sphere, saturnTexture);
	planet->T = 5.0f;
	planet->A = 100.0f;
	planet->¦Å = 0.517f;
	planet->I = 0.0f;
	planet->¦Ø = 102.3f;
	planet->¦¸ = 0.0f;
	planet->M = 0.0f;
	planet->UpdatePosition(glfwGetTime());

	sun = new PlanetObject(glm::vec3(0.0f), sphere, sunTexture);

	camera.BindUniformBuffer(0);
}

void Solar::ProcessInput(GLfloat dt){
	if (InputManager::isKeyPressed(GLFW_KEY_W))
	{
		camera.ProcessKeyBoard(FORWARD, dt);
	}
	if (InputManager::isKeyPressed(GLFW_KEY_S))
	{
		camera.ProcessKeyBoard(BACKWARD, dt);
	}
	if (InputManager::isKeyPressed(GLFW_KEY_A))
	{
		camera.ProcessKeyBoard(LEFT, dt);
	}
	if (InputManager::isKeyPressed(GLFW_KEY_D))
	{
		camera.ProcessKeyBoard(RIGHT, dt);
	}
	camera.ProcessMouseMovement(InputManager::DeltaX, InputManager::DeltaY);
	camera.ProcessMouseScroll(InputManager::MouseScroll);
}

void Solar::Update(GLfloat dt){
	planet->UpdatePosition(glfwGetTime());
}

void Solar::Render(){
	Shader ourShader = ResourceManager::GetShader("basic").Use();

	glm::mat4 model;
	model = glm::translate(model, planet->Position);
	model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	ourShader.SetMatrix4("model", model);

	std::cout << planet->Position.x <<"-"<< planet->Position.y <<"-"<< planet->Position.z << std::endl;

	glActiveTexture(GL_TEXTURE0);
	planet->texture->Bind();
	ourShader.SetInteger("texture0", 0);

	glBindVertexArray(planet->mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, planet->mesh->vertices.size());
	glBindVertexArray(0);

	model = glm::mat4();
	model = glm::translate(model, sun->Position);
	model = glm::scale(model, glm::vec3(20.0f));
	ourShader.SetMatrix4("model", model);
	glActiveTexture(GL_TEXTURE1);
	sun->texture->Bind();
	ourShader.SetInteger("texture0", 1);
	glBindVertexArray(sun->mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, sun->mesh->vertices.size());
	glBindVertexArray(0);
}
