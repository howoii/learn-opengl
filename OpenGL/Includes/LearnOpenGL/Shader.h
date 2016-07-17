#ifndef SHADER_H
#define SHADER_H

#include "OpenGLHeader.h"

class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode = lglLoadStringFromFile(vertexPath);
		std::string fragmentCode = lglLoadStringFromFile(fragmentPath);
		
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();

		// 2. Compile shaders
		GLuint vertex, fragment;
		vertex = lglCreateShaderStage(GL_VERTEX_SHADER, vShaderCode);
		fragment = lglCreateShaderStage(GL_FRAGMENT_SHADER, fShaderCode);

		// 3. Shader Program
		this->Program = lglCreateShaderProgram(vertex, fragment);

		// 4. Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar *geometryPath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode = lglLoadStringFromFile(vertexPath);
		std::string fragmentCode = lglLoadStringFromFile(fragmentPath);
		std::string geometryCode = lglLoadStringFromFile(geometryPath);

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();
		const GLchar * gShaderCode = geometryCode.c_str();

		// 2. Compile shaders
		GLuint vertex, fragment, geometry;
		vertex = lglCreateShaderStage(GL_VERTEX_SHADER, vShaderCode);
		fragment = lglCreateShaderStage(GL_FRAGMENT_SHADER, fShaderCode);
		geometry = lglCreateShaderStage(GL_GEOMETRY_SHADER, gShaderCode);
		// 3. Shader Program
		this->Program = lglCreateShaderProgram(vertex, fragment, geometry);

		// 4. Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);
	}

	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}

	void BindUniformBlock(const GLchar *uboName, GLuint bindPoint){
		GLuint uniformBlockIndex = glGetUniformBlockIndex(this->Program, uboName);
		glUniformBlockBinding(this->Program, uniformBlockIndex, bindPoint);
	}
};

#endif