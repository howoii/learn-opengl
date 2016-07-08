#include "OpenGLHeader.h"
#include "Shader.h"

#include <iostream>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void do_movement();

static glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};
static glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
static glm::vec3 cameraDirection(0.0f, 0.0, -1.0f);
static glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
static glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
static GLfloat deltaTime = 0.0f;
static GLfloat lastTime = 0.0f;
static GLfloat fov = 45.0f;
static GLfloat aspectRatio = GLfloat(LGL_WINDOW_WIDTH) / LGL_WINDOW_HEIGHT;

bool keys[1024];

int main(){
	lglGlfwInit(3, 3, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(LGL_WINDOW_WIDTH, LGL_WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	lglGlewInit();

	glViewport(0, 0, LGL_WINDOW_WIDTH, LGL_WINDOW_HEIGHT);

	Shader ourShader("textures.vs", "textures.frag");

	GLfloat vertices[] = {
		// Positions   // Texture Coordinates
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	GLuint VAO, VBO;
	GLint attrSize[] = {3, 2};

	lglCreateVertexArray(&VAO, &VBO, vertices, sizeof(vertices), attrSize, 2);

	GLuint texture1, texture2;
	lglCreateTexture(&texture1, GL_TEXTURE_2D, "wood.png");
	lglCreateTexture(&texture2, GL_TEXTURE_2D, "face.png");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat curTime = glfwGetTime();
		deltaTime = curTime - lastTime;
		lastTime = curTime;

		glfwPollEvents();
		do_movement();
		//render BEGIN
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

		//cameras view
		glm::mat4 view;
		cameraTarget = cameraPos + cameraDirection;
		view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(curTime*45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		//render END
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}

void do_movement(){
	GLfloat moveSpeed = 4.0f * deltaTime;
	GLfloat rotateSpeed = 0.5f * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		cameraPos += glm::normalize(cameraDirection) * moveSpeed;
	}
	if (keys[GLFW_KEY_S])
	{
		cameraPos -= glm::normalize(cameraDirection) * moveSpeed;
	}
	if (keys[GLFW_KEY_D])
	{
		cameraPos += glm::normalize(glm::cross(cameraDirection, cameraUp)) * moveSpeed;
	}
	if (keys[GLFW_KEY_A])
	{
		cameraPos -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * moveSpeed;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	GLfloat moveSpeed = 0.2f;
	GLfloat rotateSpeed = 0.05f;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}