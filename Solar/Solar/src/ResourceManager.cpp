#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, TextureCube> ResourceManager::CubeMaps;
std::map<std::string, Mesh> ResourceManager::Meshes;
std::map<std::string, FloatTable> ResourceManager::Parameters;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name){
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}
Shader ResourceManager::GetShader(std::string name){
	return Shaders[name];
}

Shader* ResourceManager::GetShaderPointer(std::string name){
	return &Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name){
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}
Texture2D ResourceManager::GetTexture(std::string name){
	return Textures[name];
}

Texture2D* ResourceManager::GetTexturePointer(std::string name){
	return &Textures[name];
}

TextureCube ResourceManager::LoadCubeMap(const GLchar *dir, GLboolean alpha, std::string name){
	CubeMaps[name] = loadCubeMapFromFile(dir, alpha);
	return CubeMaps[name];
}

TextureCube ResourceManager::GetCubeMap(std::string name){
	return CubeMaps[name];
}

TextureCube *ResourceManager::getCubeMapPointer(std::string name){
	return &CubeMaps[name];
}

Mesh ResourceManager::StoreMesh(Mesh mesh, std::string name){
	Meshes[name] = mesh;
	return Meshes[name];
}

Mesh ResourceManager::GetMesh(std::string name){
	return Meshes[name];
}

Mesh* ResourceManager::GetMeshPointer(std::string name){
	return &Meshes[name];
}

FloatTable ResourceManager::LoadParameter(const GLchar *file, std::string name){
	Parameters[name] = loadParameterFromFile(file);
	return Parameters[name];
}

FloatTable ResourceManager::GetParameter(std::string name){
	return Parameters[name];
}


void ResourceManager::Clear(){
	for (auto iter : Shaders)
	{
		glDeleteProgram(iter.second.ID);
	}
	for (auto iter : Textures)
	{
		glDeleteTextures(1, &iter.second.ID);
	}
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile){
	std::string vertexCode, fragmentCode, geometryCode;

	try
	{
		std::ifstream vertexFile(vShaderFile);
		std::ifstream fragmentFile(fShaderFile);

		std::stringstream vertexString, fragmentString;

		vertexString << vertexFile.rdbuf();
		fragmentString << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexString.str();
		fragmentCode = fragmentString.str();

		if (gShaderFile != nullptr)
		{
			std::ifstream geometryFile(gShaderFile);
			std::stringstream geometryString;
			geometryString << geometryFile.rdbuf();
			geometryFile.close();
			geometryCode = geometryString.str();
		}
	}
	catch (std::exception* e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha){
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	int width, height;
	unsigned char *image = SOIL_load_image(file, &width, &height, 0, texture.Internal_Format != GL_RGBA ? SOIL_LOAD_RGB : SOIL_LOAD_RGBA);
	texture.Generate(width, height, image);
	SOIL_free_image_data(image);
	return texture;
}

TextureCube ResourceManager::loadCubeMapFromFile(const GLchar *dir, GLboolean alpha)
{
	TextureCube cubemap;
	if (alpha)
	{
		cubemap.Internal_Format = GL_RGBA;
		cubemap.Image_Format = GL_RGBA;
	}
	std::vector<std::string> faces;
	std::string textureDir(dir);
	faces.push_back(textureDir + "/right.jpg");
	faces.push_back(textureDir + "/left.jpg");
	faces.push_back(textureDir + "/top.jpg");
	faces.push_back(textureDir + "/bottom.jpg");
	faces.push_back(textureDir + "/back.jpg");
	faces.push_back(textureDir + "/front.jpg");

	cubemap.Generate(faces);
	return cubemap;
}

FloatTable ResourceManager::loadParameterFromFile(const GLchar *file){
	FloatTable parameters;

	GLfloat parameter;
	std::string line;
	std::ifstream fstream(file);

	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			if (line[0] == '#')
				continue;
			std::stringstream sstream(line);
			std::vector<GLfloat> row;
			while (sstream >> parameter)
			{
				row.push_back(parameter);
			}
			if (row.size() != 0)
				parameters.push_back(row);
		}
		return parameters;
	}
}