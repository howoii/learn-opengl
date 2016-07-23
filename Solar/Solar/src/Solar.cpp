#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "PlanetObject.h"
#include "SolarMath.h"


Camera camera(GLfloat(4)/3, 0.0, 0.0, glm::vec3(-30.0f, 10.0f, 100.0f));
PlanetObject *planet;

Solar::Solar(GLuint width, GLuint height)
	:Width(width), Height(height)
{

}

Solar::~Solar(){

}

void Solar::Init(){
	Shader ourShader = ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic");
	ourShader.SetUniformBlock("Camera", 0);
	ResourceManager::LoadTexture("textures/planet.png", GL_FALSE, "saturn");
	ResourceManager::StoreMesh(Mesh::GetSphereMesh(), "sphere");

	Mesh *sphere = &ResourceManager::Meshes["sphere"];
	Texture2D *saturnTexture = &ResourceManager::Textures["saturn"];

	planet = new PlanetObject(glm::vec3(0.0f), sphere, saturnTexture);
	planet->T = 1.0f;
	planet->A = 80.0f;
	planet->¦Å = 0.8f;
	planet->I = 50.0f;
	planet->¦Ø = 0.0f;
	planet->¦¸ = 0.0f;
	planet->M = 0.0f;
	planet->UpdatePosition(glfwGetTime());

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
	model = glm::scale(model, glm::vec3(20.0f));
	ourShader.SetMatrix4("model", model);

	std::cout << planet->Position.x <<"-"<< planet->Position.y <<"-"<< planet->Position.z << std::endl;

	glActiveTexture(GL_TEXTURE0);
	planet->texture->Bind();
	ResourceManager::GetShader("basic").SetInteger("texture0", 0);

	glBindVertexArray(planet->mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, planet->mesh->vertices.size());
	glBindVertexArray(0);
}
