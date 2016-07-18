#ifndef OPENGLHEADER_H
#define OPENGLHEADER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;

const GLint SHADOW_WIDTH = 1024;
const GLint SHADOW_HEIGHT = 1024;
/*
 * Initialize the GLFW window
*/
inline GLvoid lglGlfwInit(GLint major, GLint minor, GLint resizable){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_SAMPLES, 4);
}

/*
 * Initialize the GLEW library
*/
inline GLvoid lglGlewInit(){
	glewExperimental = GL_TRUE;
	glewInit();
}

inline GLuint lglCreateBuffer(GLfloat vertices[], GLuint size){
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	return VBO;
}

/*
 * attrSize: GLint[] consists of the length of each attributes
 * len     : length of attrSize array
 */
inline GLuint lglCreateVertexArray(GLuint VBO, GLint attrSize[], GLuint len){
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLint stride = 0;
	GLint offset = 0;
	for (GLuint i = 0; i < len; i++)
	{
		stride += attrSize[i];
	}

	for (GLuint i = 0; i < len; i++)
	{
		glVertexAttribPointer(i, attrSize[i], GL_FLOAT, GL_FALSE, stride*sizeof(GLfloat),(GLvoid*)(offset*sizeof(GLfloat)));
		glEnableVertexAttribArray(i);
		offset += attrSize[i];
	}
	glBindVertexArray(0);

	return VAO;
}

inline GLuint lglCreateColorBuffer(GLsizei width, GLsizei height){
	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texColorBuffer;
}

inline GLuint lglCreateDepthBuffer(GLsizei width, GLsizei height){
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	return depthMap;
}

inline GLuint lglCreateRenderBuffer(GLsizei width, GLsizei height){
	GLuint RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return RBO;
}

inline GLuint lglCreateFrameBuffer(GLuint colorBuffer, GLuint renderBuffer){
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMERBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return FBO;
}

inline GLuint lglCreateUniformBuffer(GLsizeiptr size){
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return UBO;
}

inline void lglUniformBufferData(GLuint UBO, GLintptr offset, GLsizeiptr size, const void *data){
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/*
 * imgName: const GLchar*
*/
inline GLuint lglLoadTexture2D(const GLchar *imgName, GLboolean alpha = GL_FALSE){
	GLuint texture;

	int width, height;
	unsigned char *image = SOIL_load_image(imgName, &width, &height, 0, alpha ? SOIL_LOAD_RGBA :SOIL_LOAD_RGB);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA :GL_RGB, width, height, 0, alpha ? GL_RGBA :GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE :GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

inline GLuint lglLoadTextureCube(std::vector<const GLchar *> faces){
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	GLint width, height;
	unsigned char *image;
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture;
}

inline GLuint lglCreateShaderStage(GLenum type, const GLchar *shaderCode){
	GLuint shader;
	GLint success;
	GLchar infoLog[512];
	
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::string shaderType;
		switch (type)
		{
		case GL_VERTEX_SHADER:
			shaderType = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "FREGMANT";
			break;
		case GL_GEOMETRY_SHADER:
			shaderType = "GEOMETRY";
			break;
		default:
			break;
		}
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<< shaderType <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

inline GLuint lglCreateShaderProgram(GLuint vertexShader, GLuint fragmentShader){
	GLuint program; 
	GLint success;
	GLchar infoLog[512];
	
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	return program;
}

inline GLuint lglCreateShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader){
	GLuint program;
	GLint success;
	GLchar infoLog[512];

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	return program;
}

inline std::string lglLoadStringFromFile(const GLchar *fileName){
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		shaderFile.open(fileName);
		std::stringstream codeStream;
		codeStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = codeStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return shaderCode;
}

#endif // !OPENGLHEADER_H
