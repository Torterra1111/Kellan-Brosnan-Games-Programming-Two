#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

Texture::Texture()
{
}

void Texture::Create(const std::string& fileName)
{
	//varibles
	int width;
	int height;
	int numofComp;

	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numofComp, 4);
	if (imageData == NULL)
	{
		std::cerr << "texture load failed" << fileName << std::endl;
	}
	glGenTextures(1, &textureHolder); // number of and address of textures
	glBindTexture(GL_TEXTURE_2D, textureHolder); //bind texture - define type & specify texture we are working with
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture outside height
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filtering for magnification (texture is larger)
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); //Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of Texture, Our Images Data

	stbi_image_free(imageData);


}

Texture::~Texture() //Destructor
{
	glDeleteTextures(1, &textureHolder);
}

void Texture::Bind(unsigned int unit) //Binding the texure for use
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, textureHolder); //type of and texture to bind to unit

}