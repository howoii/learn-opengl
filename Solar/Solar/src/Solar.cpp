#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "PlanetObject.h"
#include "Sun.h"
#include "SolarMath.h"
#include "PointLightRenderer.h"


Camera camera(GLfloat(4)/3, 0.0, 0.0, glm::vec3(0.0f, 60.0f, 0.0f));
PointLightRenderer *PLRenderer;
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

	ResourceManager::LoadShader("shaders/basic.vs", "shaders/pointLight.frag", nullptr, "pointLight").SetUniformBlock("camera", 0);
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
	sun = Sun(mesh, ResourceManager::GetTexturePointer("sun"), planetPara.back());

	PLRenderer = new PointLightRenderer(ResourceManager::GetShaderPointer("pointLight"),
		glm::vec3(0.0f), 0.02f, sun.Brightness/2.0f);
	
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
		//planets[i].UpdatePosition(glfwGetTime());
		planets[i].UpdateRotation(glfwGetTime());
	}
}

void Solar::Render(){
	for (GLint i=0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		PLRenderer->Render(&planets[i], planets[i].Reflect, camera.Position);
	}

	SObject *obj = &sun;
	Shader basicShader = ResourceManager::GetShader("basic").Use();
	basicShader.SetFloat("brightness", sun.Brightness);
	obj->Draw(basicShader);
}
