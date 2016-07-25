#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "PlanetObject.h"
#include "Sun.h"
#include "SolarMath.h"


Camera camera(GLfloat(4)/3, 0.0, 0.0, glm::vec3(0.0f, 0.0f, 600.0f));
std::vector<PlanetObject> planets;
Sun sun;

Solar::Solar(GLuint width, GLuint height)
	:Width(width), Height(height)
{

}

Solar::~Solar(){

}

void Solar::Init(){

	std::string textureDir = "textures";
	std::string planetNames[SOLAR_PLANET_NUMBERS] = { "mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune" };

	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		std::string textureName = planetNames[i];
		std::string texturePath = textureDir + "/planet_textures/texture_" + textureName + ".jpg";
		ResourceManager::LoadTexture(texturePath.c_str(), GL_FALSE, textureName.c_str());
	}
	ResourceManager::LoadTexture("textures/planet_textures/texture_sun.jpg", GL_FALSE, "sun");

	ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic").SetUniformBlock("camera", 0);

	ResourceManager::StoreMesh(Mesh::GetSphereMesh(), "sphere");
	ResourceManager::StoreMesh(Mesh::GetCubeMesh(), "cube");
	ResourceManager::StoreMesh(Mesh::GetPlaneMesh(), "plane");

	FloatTable planetPara = ResourceManager::LoadParameter("configure/PlanetParameter.txt", "planetPara");

	Mesh *mesh = ResourceManager::GetMeshPointer("sphere");
	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		Texture2D *texture = ResourceManager::GetTexturePointer(planetNames[i]);
		planets.push_back(PlanetObject(mesh, texture, planetPara[i]));
	}

	sun = Sun(mesh, ResourceManager::GetTexturePointer("sun"), planetPara.back()[0]);

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
	for (GLint i=0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		planets[i].UpdatePosition(glfwGetTime());
		planets[i].UpdateRotation(glfwGetTime());
	}
}

void Solar::Render(){
	Shader ourShader = ResourceManager::GetShader("basic").Use();

	for (GLint i=0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, planets[i].Position * glm::vec3(SOLAR_AXIS_SCALE));
		model = glm::rotate(model, glm::radians(planets[i].O), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(planets[i].Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(planets[i].R * SOLAR_PLANET_SCALE * SOLAR_AXIS_SCALE));
		ourShader.SetMatrix4("model", model);
	
		glActiveTexture(GL_TEXTURE0);
		planets[i].texture->Bind();
		ourShader.SetInteger("texture0", 0);

		glBindVertexArray(planets[i].mesh->VAO);
		glDrawArrays(GL_TRIANGLES, 0, planets[i].mesh->vertices.size());
		glBindVertexArray(0);
	}

	//std::cout << planet->Position.x <<"-"<< planet->Position.y <<"-"<< planet->Position.z << std::endl;


	glm::mat4 model = glm::mat4();
	model = glm::scale(model, glm::vec3(sun.Radius * SOLAR_SUN_SCALE * SOLAR_AXIS_SCALE));
	ourShader.SetMatrix4("model", model);
	glActiveTexture(GL_TEXTURE0);
	sun.texture->Bind();
	ourShader.SetInteger("texture0", 0);
	glBindVertexArray(sun.mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, sun.mesh->vertices.size());
	glBindVertexArray(0);
}
