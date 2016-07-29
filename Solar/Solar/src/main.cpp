#include "Common.h"
#include "Solar.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void cursor_enter_callback(GLFWwindow* window, int entered);

Solar solar(SCREEN_WIDTH, SCREEN_HEIGHT);

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
	glfwSetCursorEnterCallback(window, cursor_enter_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);

	//---------------------------test code----------------------------
	
	//----------------------------------------------------------------

	solar.Init();

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		solar.ProcessInput(deltaTime);

		solar.Update(deltaTime);

		solar.Render();
		//-------------------------test code-------------------------------
		
		//------------------------------------------------------------------
		InputManager::ClearMouseMovement();
		InputManager::ClearMouseScroll();

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
	InputManager::ProcessKeyInput(key, action);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	InputManager::PorcessScrollInput(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	InputManager::ProcessMouseInput(xpos, ypos);
}

void cursor_enter_callback(GLFWwindow* window, int entered){
	if (!entered)
	{
		InputManager::ProcessMouseEnter();
	}
}