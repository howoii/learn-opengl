#include "ShadowRender.h"

ShadowRender::ShadowRender(){
}

ShadowRender::~ShadowRender(){
}

ShadowRender::ShadowRender(glm::vec3 lightDirection, GLfloat edge, GLfloat near, GLfloat far, Shader *shader)
	:DepthMap(), DepthShader(shader), LightDirection(LightDirection), Edge(edge), NearPlane(near), FarPlane(far)
{
	this->setupFrameBuffer();
	this->setupLightMatrix();
}

void ShadowRender::setupFrameBuffer()
{
	glGenFramebuffers(1, &this->DepthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->DepthFBO);

	this->DepthMap.Internal_Format = GL_DEPTH_COMPONENT;
	this->DepthMap.Image_Format = GL_DEPTH_COMPONENT;
	this->DepthMap.DataType = GL_FLOAT;
	this->DepthMap.Wrap_S = GL_CLAMP_TO_BORDER;
	this->DepthMap.Wrap_T = GL_CLAMP_TO_BORDER;
	this->DepthMap.Generate(SHADOW_WIDTH, SHADOW_HEIGHT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->DepthMap.ID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowRender::setupLightMatrix(){
	glm::vec3 eye = glm::vec3(0.0f, (FarPlane - NearPlane)/2.0f, 0.0f);
	glm::mat4 lightProjection = glm::ortho(-Edge, Edge, -Edge, Edge, NearPlane, FarPlane);
	glm::mat4 lightView = glm::lookAt(eye, eye + this->LightDirection, glm::vec3(0.01f, 1.0f, 0.01f));
	this->LightSpaceMatrix = lightProjection * lightView;
}

void ShadowRender::BeginRender(){
	this->DepthShader->Use();
	this->DepthShader->SetMatrix4("lightSpaceMatrix", this->LightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, this->DepthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowRender::EndRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowRender::UpdateDirection(glm::vec3 direction){
	this->LightDirection = direction;
	this->setupLightMatrix();
}