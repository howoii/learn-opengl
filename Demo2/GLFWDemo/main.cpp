#define GLEW_STATIC //静态编译
#include <GL/glew.h> //glew.h会引入OpenGL头文件，因此要放在其他引用之前
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\n\0";

//声明一个uniform vec4的ourColor,从主程序向Shader传递颜色
const GLchar * fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用OpenGL核心模式
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//创建窗口
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//注册回调函数
	glfwSetKeyCallback(window, key_callback);

	//GLEW初始化，GLEW是用来管理OpenGL的函数指针的,调用任何OpenGL的函数之前我们需要初始化GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	//GLEW初始化结束

	GLint width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height); //渲染窗口的位置和大小

	GLuint vertexShader; //创建顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//将着色器源码附加到着色器对象，然后编译
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram; //创建着色器程序
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); //链接着色器到程序
	glDeleteShader(vertexShader); //链接完成后删除着色器
	glDeleteShader(fragmentShader);

	//顶点数据(x,y,z),z可以理解为距离或深度
	GLfloat vertices[] = {
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	//顶点缓冲对对象，在GPU内存(通常被称为显存)中储存大批顶点
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//顶点数组对象，随后的顶点属性调用和VBO都会储存在这个VAO中
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	//将新建的缓冲绑定到GL_ARRAY_BUFFER上，其中后者是一种缓冲类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据复制到绑定的缓冲
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//顶点颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//游戏循环
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); //检查有没有触发什么事件,然后调用对应的回调函数

		//执行渲染开始
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //设置清空缓冲的颜色
		glClear(GL_COLOR_BUFFER_BIT);
		
		//--绘制图形--
		glUseProgram(shaderProgram); //激活着色器程序
		/*
		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2 )+0.5;
		//获取uniform 变量ourColor的地址
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //设置uniform变量的值
		*/

		glBindVertexArray(VAO);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //使用填充（默认）模式绘制
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		//执行渲染结束

		glfwSwapBuffers(window); 
		/*应用双缓冲区渲染窗口应用程序，
		  前面的缓冲区用于显示
		  后面的用于计算，当渲染命令执行结束后
		  交换前后台缓冲区，图像就显示出来了
		 */
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate(); //释放GLFW申请的内存
	return 0;
}

//按键回调函数
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}