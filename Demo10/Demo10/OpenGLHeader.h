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

const GLuint LGL_WINDOW_WIDTH = 800;
const GLuint LGL_WINDOW_HEIGHT = 600;
/*
 * Initialize the GLFW window
*/
inline GLvoid lglGlfwInit(GLint major, GLint minor, GLint resizable){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
}

/*
 * Initialize the GLEW library
*/
inline GLvoid lglGlewInit(){
	glewExperimental = GL_TRUE;
	glewInit();
}

inline GLvoid lglCreateBuffer(GLuint *VBO, GLfloat vertices[], GLuint size){
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

/*
 * vertices: GLfloat[] vertex data array
 * size    : size of vertices array
 * attrSize: GLint[] consists of the length of each attributes
 * len     : length of attrSize array
 */
inline GLvoid lglCreateVertexArray(GLuint *VAO, GLuint VBO, GLint attrSize[], GLuint len){
	glGenVertexArrays(1, VAO);

	glBindVertexArray(*VAO);

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
}

/*
 * imgName: const GLchar*
*/
inline GLuint lglLoadTexture2D(const GLchar *imgName){
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char *image = SOIL_load_image(imgName, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

#endif // !OPENGLHEADER_H
