// Std. Includes
#include "LearnOpenGL/OpenGLHeader.h"

#include "LearnOpenGL/Shader.h"
#include "LearnOpenGL/Camera.h"
#include "LearnOpenGL/Cube.h"
#include "LearnOpenGL/Plane.h"
#include "LearnOpenGL/Texture.h"
#include "PointLight.h"
#include "DirectLight.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(0.0f, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

//Light
PointLight pointLight(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f), POINT_LIGHT100);
DirectLight dirLight(glm::vec3(-1.0f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f));

GLfloat shininess = 16.0f;

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	lglGlfwInit(3, 3, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	lglGlewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//vertices data
	Plane ground(10.f, 10.0f);
	Cube box(0.5f);
	Plane screen;

	//Texture binding
	Texture texture_wood("textures/wood.png");
	Texture texture_box("textures/container.png");

	//Create Shader
	Shader groundShader("shaders/ground.vs", "shaders/ground.frag");
	Shader depthShader("shaders/depth.vs", "shaders/depth.frag");
	Shader screenShader("shaders/screen.vs", "shaders/screen.frag");

	groundShader.BindUniformBlock("Camera",0);

	camera.BindUniformBuffer(0);

	//create depth frame buffer
	GLuint depthFBO, depthMap;
	glGenFramebuffers(1, &depthFBO);
	depthMap = lglCreateDepthBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//create lightSpaceMatrix
	glm::mat4 lightProjectin = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(6.0f, 2.0f, 6.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjectin  * lightView;

	// Setup some OpenGL options
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		//generate depth texture
		depthShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		box.translate(0.0f, 0.25f, 0.0f);
		box.Draw(depthShader);
		ground.Draw(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clear the colorbuffer
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		groundShader.Use();
		pointLight.Apply(groundShader, "pointLight");
		dirLight.Apply(groundShader, "dirLight");
		glUniform1f(glGetUniformLocation(groundShader.Program, "shininess"), shininess);
		glUniform3f(glGetUniformLocation(groundShader.Program, "viewPos"), 
			camera.Position.x, camera.Position.y, camera.Position.z);

		glUniformMatrix4fv(glGetUniformLocation(groundShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(groundShader.Program, "shadowMap"), 2);

		texture_wood.Active(0);
		texture_box.Active(1);

		texture_wood.Apply(groundShader, "Diffuse", 3.0f);
		texture_wood.Apply(groundShader, "Specular", 3.0f);
		ground.Draw(groundShader);

		texture_box.Apply(groundShader, "Diffuse");
		texture_box.Apply(groundShader, "Specular");
		box.translate(0.0f, 0.25f, 0.0f);
		box.Draw(groundShader);

		screenShader.Use();
		glUniform1i(glGetUniformLocation(screenShader.Program, "texture0"), 2);
		screen.Draw(screenShader);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyBoard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyBoard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyBoard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyBoard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion