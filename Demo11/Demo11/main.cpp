// Std. Includes
#include "LearnOpenGL/OpenGLHeader.h"

#include "LearnOpenGL/Shader.h"
#include "LearnOpenGL/Camera.h"
#include "LearnOpenGL/Cube.h"
#include "LearnOpenGL/Plane.h"
#include "LearnOpenGL/Texture.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(0.0f, 0.0f, glm::vec3(0.0f, 1.0f, 1.0f));
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
	Cube skyBox(10.0f);
	Cube container;
	Plane ground(10.0f, 10.0f);
	Plane grass(0.5f, 1.0f);
	Plane windowPlane;

	//Create Texture
	std::vector<const GLchar *> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");

	Texture texture_skybox(faces);
	Texture texture_container("textures/container.png");
	Texture texture_grass("textures/grass.png", GL_TRUE);
	Texture texture_ground("textures/grassGround.png");
	Texture texture_window("textures/window.png", GL_TRUE);

	//Texture binding
	texture_skybox.Active(0);
	texture_container.Active(1);
	texture_grass.Active(2);
	texture_ground.Active(3);
	texture_window.Active(4);

	//Create Shader
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.frag");
	Shader containerShader("shaders/container.vs", "shaders/container.frag");
	Shader planeShader("shaders/plane.vs", "shaders/plane.frag");
	Shader screenShader("shaders/screen.vs", "shaders/screen.frag");

	skyboxShader.BindUniformBlock("Transform", 0);
	containerShader.BindUniformBlock("Transform", 0);

	camera.BindUniformBuffer(0);

	/////////////////////////////////////////////////////////////
	// Create a frame buffer with color,depth and stencil buffer
	/////////////////////////////////////////////////////////////
	//Texture Buffer
	GLuint texColorBuffer = lglCreateColorBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	//Render Buffer
	GLuint RBO = lglCreateRenderBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	//Frame Buffer
	GLuint FBO = lglCreateFrameBuffer(texColorBuffer, RBO);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		/////////////////////////////////////////////////////
		// First render pass: Mirror texture...
		// Bind to framebuffer and draw to color texture as 
		// we normally would, but with the view camera 
		// reversed.
		// //////////////////////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		camera.Reverse();

		//draw skybox
		skyboxShader.Use();
		texture_skybox.Apply(skyboxShader, "skybox");
		skyBox.Draw(skyboxShader);

		containerShader.Use();
		texture_container.Apply(containerShader, "texture0");
		container.translate(0.0f, 0.51f, 0.0f);
		container.Draw(containerShader);

		/////////////////////////////////////////////////////
		// Second render pass: Draw as normal
		// //////////////////////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Reverse();

		skyboxShader.Use();
		texture_skybox.Apply(skyboxShader, "skybox");
		skyBox.Draw(skyboxShader);

		containerShader.Use();
		texture_container.Apply(containerShader, "texture0");
		container.translate(0.0f, 0.51f, 0.0f);
		container.Draw(containerShader);
		/*
		planeShader.Use();

		camera.Attach(planeShader, GLfloat(WINDOW_WIDTH) / WINDOW_HEIGHT);

		texture_ground.Apply(planeShader, "texture0", 5.0f);
		ground.translate(0.0f, 0.0f, 0.0f);
		ground.Draw(planeShader);

		texture_grass.Apply(planeShader, "texture0");
		grass.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		grass.translate(0.0f, 0.5f, 2.0f);
		grass.Draw(planeShader);
		*/

		//Draw plane using texColorBuffer as texture
		screenShader.Use();
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glUniform1i(glGetUniformLocation(screenShader.Program, "texture0"), 5);
		windowPlane.Draw(screenShader);


		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glDeleteFramebuffers(1, &FBO);

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