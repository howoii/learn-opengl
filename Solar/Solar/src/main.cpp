#include "Common.h"
#include "Solar.h"
#include "ResourceManager.h"

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

//////////////////////////////////////////////
glm::vec3 viewPos(0.0f, 0.0f, -5.0f);
GLfloat fov = 45.0f;
GLfloat aspectRatio = GLfloat(SCREEN_WIDTH) / SCREEN_HEIGHT;
//////////////////////////////////////////////

Camera camera(GLfloat(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.0, 0.0, glm::vec3(0.0f, 0.0f, 200.0f));
//Solar solar(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[]){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Solar", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);

	//----------------------------------------------------------------
	Shader ourShader = ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.frag", nullptr, "basic");
	ResourceManager::LoadTexture("textures/planet.png", GL_FALSE, "planet");
	
	Mesh cube = Mesh::GetCubeMesh();
	Mesh plane = Mesh::GetPlaneMesh();
	Mesh sphere = Mesh::GetSphereMesh(1.0f);
	//----------------------------------------------------------------

	//solar.Init();

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		GLfloat FPS = 1.0f / deltaTime;
		std::cout << "FPS:" << FPS << std::endl;

		glfwPollEvents();

		//solar.ProcessInput(deltaTime);

		//solar.Update(deltaTime);

		//solar.Render();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-------------------------test code-------------------------------
		ourShader.Use();

		ourShader.SetMatrix4("view", camera.GetViewMatrix());
		ourShader.SetMatrix4("projection", camera.GetProjectionMatrix());

		glm::mat4 model;
		model = glm::rotate(model, glm::radians(30.0f * currentFrame), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(60.0f));
		ourShader.SetMatrix4("model", model);

		glActiveTexture(GL_TEXTURE0);
		ResourceManager::GetTexture("planet").Bind();
		ResourceManager::GetShader("basic").SetInteger("texture0", 0);

		glBindVertexArray(sphere.VAO);
		glDrawArrays(GL_TRIANGLES, 0, sphere.vertices.size());
		glBindVertexArray(0);

		//------------------------------------------------------------------

		glfwSwapBuffers(window);
	}

	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	/*
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			solar.Keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE)
		{
			solar.Keys[key] = GL_FALSE;
		}
	}
	*/
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

}