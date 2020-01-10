#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture();
	
	void Create(const std::string& fileName); //Creating the texure
	void Bind(unsigned int unit); // bind upto 32 textures

	~Texture(); //deconstructor


protected:
private:

	GLuint textureHolder; //Storage of texture
};
