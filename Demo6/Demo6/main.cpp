#include "OpenGLHeader.h"
#include "Camera.h"
#include "Shader.h"

typedef struct Light
{
	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

void key_callback(GLFWwindow *window, GLint key, GLint scacode, GLint action, GLint mode);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xDelta, double yDelta);
void do_movement();

static Camera myCamera(0.0f, 0.0f);
static bool keys[1024];
static GLfloat lastTime = 0.0f;
static GLfloat deltaTime = 0.0f;
static double xLastPos = 0.0f, yLastPos = 0.0f;
static bool firstMouse = true;

glm::vec3 cubes[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(5.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 5.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 5.0f),
	glm::vec3(-5.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, -5.0f),
	glm::vec3(0.0f, -5.0f, 0.0f),
};

glm::vec3 lightColor(1.0f);

Light light;

int main(){
	lglGlfwInit(3, 3, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(LGL_WINDOW_WIDTH, LGL_WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	lglGlewInit();
	glViewport(0, 0, LGL_WINDOW_WIDTH, LGL_WINDOW_HEIGHT);

	Shader myShader("vertex.shader", "fragment.shader");
	Shader lightShader("lightVertex.shader", "lightFrag.shader");

	GLfloat vertices[] = {
		// Positions			// Normal
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint VBO, VAO;
	GLint attrSize[] = {3, 3, 2};
	lglCreateBuffer(&VBO, vertices, sizeof(vertices));
	lglCreateVertexArray(&VAO, VBO, attrSize, 3);

	GLuint texture1, texture2;
	lglCreateTexture(&texture1, GL_TEXTURE_2D, "container.png");
	lglCreateTexture(&texture2, GL_TEXTURE_2D, "container_specular.png");

	myCamera.MouseSensivity = 0.02f;

	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(0.8f);
	light.specular = glm::vec3(0.7f);
	light.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	light.position = glm::vec3(10.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//start
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat curTime = glfwGetTime();
		deltaTime = curTime - lastTime;
		lastTime = curTime;

		do_movement();

		//camera matrix
		glm::mat4 view = myCamera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(myCamera.Zoom), GLfloat(LGL_WINDOW_WIDTH) / LGL_WINDOW_HEIGHT, 0.1f, 100.0f);

		//texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//cube shader
		myShader.Use();

		glUniform1i(glGetUniformLocation(myShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(myShader.Program, "material.specular"), 1);
		glUniform1f(glGetUniformLocation(myShader.Program, "material.shininess"), 128.0f);
		
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(myShader.Program, "viewPos"), myCamera.Position.x, myCamera.Position.y, myCamera.Position.z);
		glUniform3f(glGetUniformLocation(myShader.Program, "light.position"), myCamera.Position.x, myCamera.Position.y, myCamera.Position.z);
		glUniform3f(glGetUniformLocation(myShader.Program, "light.spotDir"), myCamera.Front.x, myCamera.Front.y, myCamera.Front.z);
		glUniform3f(glGetUniformLocation(myShader.Program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(glGetUniformLocation(myShader.Program, "light.inner"), cos(glm::radians(15.0f)));
		glUniform1f(glGetUniformLocation(myShader.Program, "light.outter"), cos(glm::radians(20.0f)));

		glUniform3f(glGetUniformLocation(myShader.Program, "light.ambient"),
			light.ambient.x, light.ambient.y, light.ambient.z);
		glUniform3f(glGetUniformLocation(myShader.Program, "light.diffuse"),
			light.diffuse.x, light.diffuse.y, light.diffuse.z);
		glUniform3f(glGetUniformLocation(myShader.Program, "light.specular"),
			light.specular.x, light.specular.y, light.specular.z);
		glUniform1f(glGetUniformLocation(myShader.Program, "light.constant"), 1.0);
		glUniform1f(glGetUniformLocation(myShader.Program, "light.linear"), 0.14);
		glUniform1f(glGetUniformLocation(myShader.Program, "light.quadratic"), 0.007);


		glBindVertexArray(VAO);
		for (int i = 0; i < 7; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubes[i]);
			model = glm::rotate(model, glm::radians(curTime * 45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//light shader
		lightShader.Use();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model2;
		model2 = glm::translate(model2, light.position);
		model2 = glm::scale(model2, glm::vec3(0.5f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));

		glUniform3f(glGetUniformLocation(lightShader.Program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//end
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos){
	if (firstMouse)
	{
		xLastPos = xPos;
		yLastPos = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - xLastPos;
	GLfloat yOffset = yLastPos - yPos;
	xLastPos = xPos;
	yLastPos = yPos;
	myCamera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xDelta, double yDelta){
	myCamera.ProcessMouseScroll(yDelta);
}

void do_movement(){
	if (keys[GLFW_KEY_W])
	{
		myCamera.ProcessKeyBoard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		myCamera.ProcessKeyBoard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		myCamera.ProcessKeyBoard(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		myCamera.ProcessKeyBoard(LEFT, deltaTime);
	}
}