#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "PlanetObject.h"
#include "Satellite.h"
#include "Sun.h"
#include "SolarMath.h"
#include "PointLight.h"
#include "DirLight.h"
#include "Ground.h"
#include "SkyBox.h"
#include "ShadowRender.h"
#include "TextRenderer.h"

Camera camera(GLfloat(4)/3, 0.0, 0.0, glm::vec3(0.0f, 60.0f, 0.0f));
PointLight pointLight;
DirLight dirLight;
std::vector<PlanetObject> planets;
PlanetObject comet;
Satellite moon;
Sun sun;
SkyBox sky;
Ground ground;
ShadowRender shadowRenderer;
TextRenderer text;
//----------------temp code-----------------
GLfloat time_scale[5] = { 1.0f, 3600.0f, 3600.0f*24.0f, 3600.0f*24.0f*30.0f, 3600.0f*24.0f*365.0f };
GLint time_scale_index = 0;
GLfloat longitude = 120.0f;
GLfloat latitude = 0.0f;
GLfloat starDistance = 50.0f;
GLfloat OutputPeriad = 1.0f;
GLfloat DeltaTime = 0.0f;
//------------------------------------------

//---------------test code------------------
GLuint hdrFBO;
GLuint rboDepth;
Texture2D colorBuffer;
PlaneObject screen;
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
	std::string planetNames[SOLAR_PLANET_NUMBERS] = { "mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune"};

	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		std::string textureName = planetNames[i];
		std::string texturePath = textureDir + "/planet_textures/texture_" + textureName + ".jpg";
		ResourceManager::LoadTexture(texturePath.c_str(), GL_FALSE, textureName.c_str());
	}
	ResourceManager::LoadTexture("textures/planet_textures/texture_sun.jpg", GL_FALSE, "sun");
	ResourceManager::LoadTexture("textures/planet_textures/texture_moon.jpg", GL_FALSE, "moon");
	ResourceManager::LoadTexture("textures/planet_textures/texture_comet.jpg", GL_FALSE, "comet");
	ResourceManager::LoadTexture("textures/Moss/mossgrown.png", GL_FALSE, "moss");
	ResourceManager::LoadCubeMap("textures/skybox", GL_FALSE, "skybox1");

	ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/basic.vs", "shaders/lightSource.frag", nullptr, "light").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/basic.vs", "shaders/dirLight.frag", nullptr, "dirLight").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/basic.vs", "shaders/pointLight.frag", nullptr, "pointLight").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.frag", nullptr, "skybox").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/star.vs", "shaders/pointLight.frag", nullptr, "star").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/sun.vs", "shaders/lightSource.frag", nullptr, "sun").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/shadow.vs", "shaders/dirLightShadow.frag", nullptr, "shadow").SetUniformBlock("camera", 0);
	ResourceManager::LoadShader("shaders/quad.vs", "shaders/hdr.frag", nullptr, "hdr");
	ResourceManager::LoadShader("shaders/depth.vs", "shaders/depth.frag", nullptr, "depth");

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
	moon = Satellite(mesh, ResourceManager::GetTexturePointer("moon"), planetPara[SOLAR_PLANET_NUMBERS]);
	moon.SetHostPlanet(&planets[SOLAR_PLANET_EARTH]);
	comet = PlanetObject(mesh, ResourceManager::GetTexturePointer("comet"), planetPara[SOLAR_PLANET_NUMBERS + 1]);
	sun = Sun(mesh, ResourceManager::GetTexturePointer("sun"), planetPara.back());

	ground = Ground(ResourceManager::GetMeshPointer("plane"), ResourceManager::GetTexturePointer("moss"), 20.0f, 20.0f);
	sky = SkyBox(ResourceManager::GetMeshPointer("cube"), ResourceManager::getCubeMapPointer("skybox1"));

	dirLight = DirLight(glm::vec3(-1.0f), SOLAR_BRIGHTNESS_LIGHT, 0.1f);
	pointLight = PointLight(glm::vec3(0.0f), SOLAR_BRIGHTNESS_LIGHT, 0.02f);
	
	shadowRenderer = ShadowRender(dirLight.Direction, 50.0f, 1.0f, 20.0f, ResourceManager::GetShaderPointer("depth"));
	text = TextRenderer(this->Width, this->Height);
	text.Load("fonts/OCRAEXT.TTF", 24);

	camera.BindUniformBuffer(0);
	//-----------------test code----------------
	glGenFramebuffers(1, &hdrFBO);

	colorBuffer.Internal_Format = GL_RGB16F;
	colorBuffer.DataType = GL_FLOAT;
	colorBuffer.Generate(this->Width, this->Height, NULL);

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->Width, this->Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// - Attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer.ID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	screen = PlaneObject(ResourceManager::GetMeshPointer("plane"), &colorBuffer);
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
	if (InputManager::isKeyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		if (InputManager::isKeyPressed(GLFW_KEY_UP))
		{
			latitude += dt * 20.0f;
			if (latitude >= 90.0f)
			{
				latitude = 89.0f;
			}
		}
		if (InputManager::isKeyPressed(GLFW_KEY_DOWN))
		{
			latitude -= dt * 20.0f;
			if (latitude <= -90.0f)
			{
				latitude = -89.0f;
			}
		}
		if (InputManager::isKeyPressed(GLFW_KEY_RIGHT))
		{
			longitude += dt * 20.0f;
			if (longitude >= 180.0f)
			{
				longitude = 179.0f;
			}
		}
		if (InputManager::isKeyPressed(GLFW_KEY_LEFT))
		{
			longitude -= dt * 20.0f;
			if (longitude <= -180.0f)
			{
				longitude = -179.0f;
			}
		}
	}
	else
	{
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
	}

	if (this->Vmode == SpaceMode)
	{
		if (InputManager::isKeyPressed(GLFW_KEY_TAB) 
			&& !InputManager::isKeyProcessed(GLFW_KEY_TAB))
		{
			this->Vmode = GroundMode;
			InputManager::ClearKeyBuffer(GLFW_KEY_TAB);
		}

		if (InputManager::isKeyPressed(GLFW_KEY_SPACE)
			&& !InputManager::isKeyProcessed(GLFW_KEY_SPACE))
		{
			camera.Reset();
			InputManager::ClearKeyBuffer(GLFW_KEY_SPACE);
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

		if (InputManager::isKeyPressed(GLFW_KEY_SPACE)
			&& !InputManager::isKeyProcessed(GLFW_KEY_SPACE))
		{
			if (TimeManager::IsStopped())
			{
				TimeManager::Continue();
			}
			else
			{
				TimeManager::Stop();
			}
			InputManager::ClearKeyBuffer(GLFW_KEY_SPACE);
		}
	}
	camera.ProcessMouseMovement(InputManager::DeltaX, InputManager::DeltaY);
	camera.ProcessMouseScroll(InputManager::MouseScroll);
	this->OutPut(dt);
}

void Solar::OutPut(GLfloat dt){
	DeltaTime += dt;
	if (DeltaTime > OutputPeriad)
	{
		DeltaTime = 0.0f;
		std::cout << "Time(UTC):	" << TimeManager::TimeStringUtc();
		std::cout << "Time(LOCAL):	" << TimeManager::TimeStringLocal(longitude);
		std::cout << "Longitude:	" << longitude << std::endl;
		std::cout << "Latitude:	" << latitude << std::endl;
		PRINT("");
	}
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
	moon.UpdatePosition(TimeManager::GetTime());
	comet.UpdatePosition(TimeManager::GetTime());
	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		planets[i].UpdateViewDirecton(&planets[SOLAR_PLANET_EARTH], longitude, latitude);
		planets[i].UpdateViewSize(&planets[SOLAR_PLANET_EARTH]);
	}
	moon.UpdateViewDirecton(&planets[SOLAR_PLANET_EARTH], longitude, latitude);
	moon.UpdateViewSize(&planets[SOLAR_PLANET_EARTH]);
	sun.UpdateViewDirecton(&planets[SOLAR_PLANET_EARTH], longitude, latitude);
	sun.UpdateViewSize(&planets[SOLAR_PLANET_EARTH]);

	dirLight.Update(sun.ViewDirection);
	sky.UpdateBrightness(sun.ViewDirection);
	shadowRenderer.UpdateDirection(dirLight.Direction);
}

void Solar::Render(){
	////--------------test code-----------------
	//Render depth buffer
	shadowRenderer.BeginRender();
	ground.Draw(*shadowRenderer.DepthShader);
	shadowRenderer.EndRender();

	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//----------------------------------------
	if (this->Vmode == SpaceMode)
	{
		this->RenderSpace();
	}
	else{
		this->RenderGround();
	}
	//-------------test code------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader hdrShader = ResourceManager::GetShader("hdr").Use();
	hdrShader.SetFloat("exposure", 0.5f);
	screen.Draw(hdrShader);
	//----------------------------------------
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
	SObject *obj;
	obj = &moon;
	obj->Draw(*planetShader);
	obj = &comet;
	obj->Draw(*planetShader);

	obj = &sun;
	Shader lightShader = ResourceManager::GetShader("light").Use();
	obj->Draw(lightShader);

	glDepthFunc(GL_LEQUAL);
	Shader skyboxShader = ResourceManager::GetShader("skybox").Use();
	sky.Draw(skyboxShader);
	glDepthFunc(GL_LESS);
}

void Solar::RenderGround(){
	//Render skybox
	glDepthFunc(GL_LEQUAL);
	Shader skyboxShader = ResourceManager::GetShader("skybox").Use();
	sky.Draw(skyboxShader);
	glDepthFunc(GL_LESS);

	//Render star
	Shader *starShader = ResourceManager::GetShaderPointer("star");
	pointLight.SetUniformData(*starShader, "pointLight", GL_TRUE);
	starShader->SetVector3f("viewPos", planets[SOLAR_PLANET_EARTH].Position);

	for (GLint i = 0; i < SOLAR_PLANET_NUMBERS; i++)
	{
		if (i == SOLAR_PLANET_EARTH) continue;
		planets[i].DrawStar(*starShader);
	}
	moon.DrawStar(*starShader);
	//Render sun
	Shader lightShader = ResourceManager::GetShader("sun").Use();
	sun.DrawStar(lightShader);

	//Render ground
	Shader *shadowShader = ResourceManager::GetShaderPointer("shadow");
	dirLight.SetUniformData(*shadowShader, "dirLight", GL_TRUE);
	shadowShader->SetVector3f("viewPos", camera.Position);
	shadowShader->SetMatrix4("lightSpaceMatrix", shadowRenderer.LightSpaceMatrix);
	glActiveTexture(GL_TEXTURE1);
	shadowRenderer.DepthMap.Bind();
	shadowShader->SetInteger("shadowMap", 1);
	ground.Draw(*shadowShader);
}