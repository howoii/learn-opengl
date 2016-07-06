#include "shader.h"

Shader::Shader(const GLchar *vertexSourcePath, const GLchar * fragmentSourcePath){
	std::string vertexCode;
	std::string fregmentCode;

	try
	{
		std::ifstream vShaderFile(vertexSourcePath);
		std::ifstream fShaderFile(fragmentSourcePath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fregmentCode = fShaderStream.str();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR: READ SHADER FILE FAILED." << std::endl;
	}
}