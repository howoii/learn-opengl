#include "TextureCube.h"

TextureCube::TextureCube()
	:Internal_Format(GL_RGB), Image_Format(GL_RGB),
	Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Wrap_R(GL_CLAMP_TO_EDGE),
	Filter_Min(GL_LINEAR), Filter_Mag(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

TextureCube::~TextureCube()
{
}

void TextureCube::Generate(std::vector<std::string> faces){
	/*this->ID = SOIL_load_OGL_cubemap(
		faces[0].c_str(),
		faces[1].c_str(),
		faces[2].c_str(),
		faces[3].c_str(),
		faces[4].c_str(),
		faces[5].c_str(),
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		);*/
	
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);

	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i].c_str(), &width, &height, 0,
			this->Internal_Format != GL_RGBA ? SOIL_LOAD_RGB : SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			this->Internal_Format, width, height, 0,
			this->Image_Format, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->Wrap_R);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
}

void TextureCube::Bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
}