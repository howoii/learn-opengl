#include "shader.h"

Shader::Shader(const GLchar *vertexSourcePath, const GLchar * fragmentSourcePath){
	GLchar *vShaderCode;
	GLchar *fShaderCode;

	try
	{
		std::ifstream vShaderFile(vertexSourcePath);
		std::ifstream fShaderFile(fragmentSourcePath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		std::string vertexCode = vShaderStream.str();
		std::string fragmentCode = fShaderStream.str();
		vShaderCode = (GLchar*)vertexCode.c_str();
		fShaderCode = (GLchar*)fragmentCode.c_str();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR: READ SHADER FILE FAILED." << std::endl;
	}

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	//¶¥µã×ÅÉ«Æ÷
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR :SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);

	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use(){
	glUseProgram(this->Program);
}