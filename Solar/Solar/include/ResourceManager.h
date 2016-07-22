#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Common.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Mesh.h"

#include <map>

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static std::map<std::string, Mesh> Meshes;

	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	static Shader GetShader(std::string name);

	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	static Texture2D GetTexture(std::string name);

	static Mesh StoreMesh(Mesh mesh, std::string name);
	static Mesh GetMesh(std::string name);

	static void Clear();

private:
	ResourceManager(){}

	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif