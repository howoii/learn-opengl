// Std. Includes
#include "LearnOpenGL/OpenGLHeader.h"

#include "LearnOpenGL/Shader.h"
#include "LearnOpenGL/Camera.h"
#include "LearnOpenGL/Cube.h"
#include "LearnOpenGL/Plane.h"
#include "LearnOpenGL/Texture.h"
#include "PointLight.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(0.0f, 0.0f, glm::vec3(0.0f, 1.0f, 1.0f));

//Light
PointLight pointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.3f), glm::vec3(2.0f), POINT_LIGHT100);

GLfloat shininess = 64.0f;

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

	//Texture binding
	Texture texture_wood("textures/wood.png");

	texture_wood.Active(0);

	//Create Shader
	Shader groundShader("shaders/ground.vs", "shaders/ground.frag");

	groundShader.BindUniformBlock("Camera",0);

	camera.BindUniformBuffer(0);

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

		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		groundShader.Use();
		pointLight.Apply(groundShader, "pointLight");
		texture_wood.Apply(groundShader, "Diffuse", 3.0f);
		texture_wood.Apply(groundShader, "Specular", 3.0f);
		glUniform1f(glGetUniformLocation(groundShader.Program, "shininess"), shininess);
		glUniform3f(glGetUniformLocation(groundShader.Program, "viewPos"), 
			camera.Position.x, camera.Position.y, camera.Position.z);
		ground.Draw(groundShader);

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