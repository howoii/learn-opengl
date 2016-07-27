#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "PlanetObject.h"
#include "Sun.h"
#include "SolarMath.h"
#include "PointLight.h"
#include "DirLight.h"
#include "Ground.h"
#include "SkyBox.h"

Camera camera(GLfloat(4)/3, 0.0, 0.0, glm::vec3(0.0f, 60.0f, 0.0f));
PointLight pointLight;
DirLight dirLight;
std::vector<PlanetObject> planets;
Sun sun;
Ground ground;
SkyBox sky;
//----------------temp code-----------------
GLfloat time_scale[5] = { 1.0f, 3600.0f, 3600.0f*24.0f, 3600.0f*24.0f*30.0f, 3600.0f*24.0f*365.0f };
GLint time_scale_index = 0;
GLfloat longitude = 0.0f;
GLfloat latitude = 0.0f;
GLfloat starDistance = 20.0f;
//------------------------------------------

//---------------test code------------------
//------------------------------------------
Solar::Solar(GLuint width, GLuint height)
	:Width(width), Height(height), Vmode(SpaceMode)
{

}

Solar::~Solar(){

}

void Solar::Init(){
	//Init Time
	TimeManager::InitTime();

	//Load Resource
	std::string textureDir = "textures";
	std::string planetNames[SOLAR_PLANET_NUMBERS] = { "mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune" };

	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		std::string textureName = planetNames[i];
		std::string texturePath = textureDir + "/planet_textures/texture_" + textureName + ".jpg";
		ResourceManager::LoadTexture(texturePath.c_str(), GL_FALSE, textureName.c_str());
	}
	ResourceManager::LoadTexture("textures/planet_textures/texture_sun.jpg", GL_FALSE, "sun");
	ResourceManager::LoadTexture("textures/Moss/mossgrown.png", GL_FALSE, "moss");
	ResourceManager::LoadCubeMap("textures/skybox", GL_FALSE, "skybox1");

	ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/basic.vs", "shaders/lightSource.frag", nullptr, "light").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/basic.vs", "shaders/dirLight.frag", nullptr, "dirLight").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/basic.vs", "shaders/pointLight.frag", nullptr, "pointLight").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/instance.vs", "shaders/dirLight.frag", nullptr, "instanceDir").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.frag", nullptr, "skybox").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/star.vs", "shaders/pointLight.frag", nullptr, "star").SetUniformBlock("camera", 0);

	ResourceManager::StoreMesh(Mesh::GetSphereMesh(), "sphere");
	ResourceManager::StoreMesh(Mesh::GetCubeMesh(), "cube");
	ResourceManager::StoreMesh(Mesh::GetPlaneMesh(), "plane");

	FloatTable planetPara = ResourceManager::LoadParameter("configure/PlanetParameter.txt", "planetPara");

	//Construct Objects
	Mesh *mesh = ResourceManager::GetMeshPointer("sphere");
	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		Texture2D *texture = ResourceManager::GetTexturePointer(planetNames[i]);
		planets.push_back(PlanetObject(mesh, texture, planetPara[i]));
	}
	sun = Sun(mesh, ResourceManager::GetTexturePointer("sun"), planetPara.back());

	ground = Ground(ResourceManager::GetMeshPointer("plane"), ResourceManager::GetTexturePointer("moss"), 20.0f, 20.0f);
	sky = SkyBox(ResourceManager::GetMeshPointer("cube"), ResourceManager::getCubeMapPointer("skybox1"));

	dirLight = DirLight(glm::vec3(-1.0f), SOLAR_BRIGHTNESS_LIGHT, 0.02f);
	pointLight = PointLight(glm::vec3(0.0f), SOLAR_BRIGHTNESS_LIGHT, 0.02f);
	
	camera.BindUniformBuffer(0);
	//-----------------test code----------------
	//------------------------------------------
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

	if (InputManager::isKeyPressed(GLFW_KEY_SPACE)
		&& !InputManager::isKeyProcessed(GLFW_KEY_SPACE))
	{
		camera.Reset();
		InputManager::ClearKeyBuffer(GLFW_KEY_SPACE);
	}

	if (InputManager::isKeyPressed(GLFW_KEY_UP)
		&& !InputManager::isKeyProcessed(GLFW_KEY_UP))
	{
		time_scale_index = (time_scale_index + 1) % 5;
		TimeManager::SetTimeScale(time_scale[time_scale_index]);
		InputManager::ClearKeyBuffer(GLFW_KEY_UP);
	}

	if (InputManager::isKeyPressed(GLFW_KEY_DOWN)
		&& !InputManager::isKeyProcessed(GLFW_KEY_DOWN))
	{
		time_scale_index = (time_scale_index - 1) % 5;
		TimeManager::SetTimeScale(time_scale[time_scale_index]);
		InputManager::ClearKeyBuffer(GLFW_KEY_DOWN);
	}

	if (this->Vmode == SpaceMode)
	{
		if (InputManager::isKeyPressed(GLFW_KEY_TAB) 
			&& !InputManager::isKeyProcessed(GLFW_KEY_TAB))
		{
			this->Vmode = GroundMode;
			InputManager::ClearKeyBuffer(GLFW_KEY_TAB);
		}
	}
	else
	{
		if (InputManager::isKeyPressed(GLFW_KEY_TAB)
			&& !InputManager::isKeyProcessed(GLFW_KEY_TAB))
		{
			this->Vmode = SpaceMode;
			InputManager::ClearKeyBuffer(GLFW_KEY_TAB);
		}
	}
	camera.ProcessMouseMovement(InputManager::DeltaX, InputManager::DeltaY);
	camera.ProcessMouseScroll(InputManager::MouseScroll);
}

void Solar::Update(GLfloat dt){
	TimeManager::UpdateTime(dt);
	//--------------test code------------------
	//-----------------------------------------
	for (GLint i=0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		planets[i].UpdatePosition(TimeManager::GetTime());
		planets[i].UpdateRotation(TimeManager::GetTime());
	}
	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		planets[i].UpdateViewDirecton(&planets[SOLAR_PLANET_EARTH], longitude, latitude);
	}
	sun.UpdateViewDirecton(&planets[SOLAR_PLANET_EARTH], longitude, latitude);

	dirLight.Update(sun.ViewDirection);
	sky.UpdateBrightness(sun.ViewDirection);
}

void Solar::Render(){
	if (this->Vmode == SpaceMode)
	{
		this->RenderSpace();
	}
	else{
		this->RenderGround();
	}
}

void Solar::RenderSpace(){
	Shader *planetShader = ResourceManager::GetShaderPointer("pointLight");
	pointLight.SetUniformData(*planetShader, "pointLight", GL_TRUE);
	planetShader->SetVector3f("viewPos", camera.Position);
	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		SObject *obj = &planets[i];
		obj->Draw(*planetShader);
	}

	SObject *obj = &sun;
	Shader lightShader = ResourceManager::GetShader("light").Use();
	obj->Draw(lightShader);

	glDepthFunc(GL_LEQUAL);
	Shader skyboxShader = ResourceManager::GetShader("skybox").Use();
	sky.Draw(skyboxShader);
	glDepthFunc(GL_LESS);
}

void Solar::RenderGround(){
	Shader *planeShader = ResourceManager::GetShaderPointer("instanceDir");
	dirLight.SetUniformData(*planeShader, "dirLight", GL_TRUE);
	planeShader->SetVector3f("viewPos", camera.Position);
	ground.Draw(*planeShader);

	//Render star
	glDepthFunc(GL_LEQUAL);

	Shader *starShader = ResourceManager::GetShaderPointer("star");
	pointLight.SetUniformData(*starShader, "pointLight", GL_TRUE);
	starShader->SetVector3f("viewPos", planets[SOLAR_PLANET_EARTH].Position);

	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		if (i == SOLAR_PLANET_EARTH) continue;

		PlanetObject planet = planets[i];
		glm::vec3 starPos = planet.ViewDirection * glm::vec3(starDistance);
		GLfloat size = 1.0;
		glm::mat4 model1;
		model1 = glm::translate(model1, starPos);
		model1 = glm::rotate(model1, glm::radians(planet.O), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(size));
		starShader->SetMatrix4("model1", model1);

		glm::mat4 model2;
		model2 = glm::translate(model2, planet.Position);
		model2 = glm::rotate(model2, glm::radians(planet.O), glm::vec3(1.0f, 0.0f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(size));
		starShader->SetMatrix4("model2", model2);

		glActiveTexture(GL_TEXTURE0);
		planet.texture->Bind();
		starShader->SetInteger("diffuse", 0);
		starShader->SetInteger("specular", 0);

		starShader->SetVector3f("material.diffuse", glm::vec3(planet.Reflect));
		starShader->SetVector3f("material.specular", glm::vec3(0.2f));
		starShader->SetFloat("material.shininess", 32.0f);

		planet.mesh->Draw(*starShader);
	}
	

	Shader skyboxShader = ResourceManager::GetShader("skybox").Use();
	sky.Draw(skyboxShader);
	glDepthFunc(GL_LESS);
}