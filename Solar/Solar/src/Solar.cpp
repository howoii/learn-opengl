#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "PlanetObject.h"
#include "Sun.h"
#include "SolarMath.h"
#include "SolarTime.h"
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
//---------------test code------------------
//------------------------------------------
Solar::Solar(GLuint width, GLuint height)
	:Width(width), Height(height), Vmode(SpaceMode)
{

}

Solar::~Solar(){

}

void Solar::Init(){
	std::string textureDir = "textures";
	std::string planetNames[SOLAR_PLANET_NUMBERS] = { "mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune" };

	//Load Resource
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

	dirLight = DirLight(glm::vec3(-1.0f), sun.Brightness, 0.02f);
	pointLight = PointLight(glm::vec3(0.0f), sun.Brightness, 0.02f);
	
	camera.BindUniformBuffer(0);
	//-----------------test code----------------
	SolarTime sTime;
	std::cout << sTime.Time2StringUTC() << std::endl;
	SolarTime eTime("2016-11-28 12:12:12");
	std::cout << eTime.Time2StringUTC() << std::endl;
	SolarTime oTime(2011, 4, 34, 1, 20, 30);
	std::cout << oTime.Time2StringUTC() << std::endl;

	sTime.AddSeconds(3600);
	std::cout << sTime.Time2StringUTC() << std::endl;
	sTime.AddHours(72);
	std::cout << sTime.Time2StringUTC() << std::endl;
	sTime.AddDays(5);
	std::cout << sTime.Time2StringUTC() << std::endl;
	sTime.AddMonths(13000);
	std::cout << sTime.Time2StringUTC() << std::endl;
	sTime.AddYears(200);
	std::cout << sTime.Time2StringUTC() << std::endl;

	std::cout << sTime.Difference(eTime) << std::endl;
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
	//--------------test code------------------
	sky.UpdateBrightness(glm::sin(glfwGetTime()/4.0f) + 1.0f);
	//-----------------------------------------
	for (GLint i=0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		planets[i].UpdatePosition(glfwGetTime());
		planets[i].UpdateRotation(glfwGetTime());
	}
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

	glDepthFunc(GL_LEQUAL);
	Shader skyboxShader = ResourceManager::GetShader("skybox").Use();
	sky.Draw(skyboxShader);
	glDepthFunc(GL_LESS);
}