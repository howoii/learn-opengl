#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"


Camera camera(GLfloat(4)/3, 0.0, 0.0, glm::vec3(0.0f, 0.0f, 150.0f));

Solar::Solar(GLuint width, GLuint height)
	:Width(width), Height(height)
{

}

Solar::~Solar(){

}

void Solar::Init(){
	Shader ourShader = ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic");
	ourShader.SetUniformBlock("Camera", 0);
	ResourceManager::LoadTexture("textures/planet.png", GL_FALSE, "planet");
	ResourceManager::StoreMesh(Mesh::GetSphereMesh(), "sphere");

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

}

void Solar::Render(){
	Shader ourShader = ResourceManager::GetShader("basic").Use();

	glm::mat4 model;
	model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f));
	ourShader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture("planet").Bind();
	ResourceManager::GetShader("basic").SetInteger("texture0", 0);

	Mesh mesh = ResourceManager::GetMesh("sphere");

	glBindVertexArray(mesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
	glBindVertexArray(0);
}
