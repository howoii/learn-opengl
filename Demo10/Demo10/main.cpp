// Std. Includes
#include "OpenGLHeader.h"

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Texture.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
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
	Cube skyBox;
	Cube container;

	std::vector<const GLchar *> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");

	Texture texture_skybox(faces);

	Texture texture_container("textures/container.png");

	Shader skyboxShader("shaders/shader.vs", "shaders/shader.frag");
	Shader containerShader("shaders/container.vs", "shaders/container.frag");
	Shader borderShader("shaders/container.vs", "shaders/pureColor.frag");

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//texture binding
		texture_skybox.Active(0);
		texture_container.Active(1);

		//view matrix and projection matrix creation
		/*glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);*/

		//draw skybox
		skyboxShader.Use();

		texture_skybox.Apply(skyboxShader, "skybox");
		camera.Attach(skyboxShader, GLfloat(WINDOW_WIDTH) / WINDOW_HEIGHT);

		skyBox.scale(6.0f);
		skyBox.Draw(skyboxShader);

		containerShader.Use();

		texture_container.Apply(containerShader, "texture0");
		camera.Attach(containerShader, GLfloat(WINDOW_WIDTH) / WINDOW_HEIGHT);

		container.scale(0.5);
		container.Draw(containerShader);

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