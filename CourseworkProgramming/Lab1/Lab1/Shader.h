#pragma once
#include <string>
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Transform.h"

class Shader 
{
public:
	Shader();

	void Bind(); //Set gpu to use our shaders
	void Update(const Transform& transform, const Camera& camera); //updating
	void Create(const std::string& filename); //creation
	
	std::string LoadShader(const std::string& fileName); //Load the shader
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage); //checking for the error
	GLuint makeShader(const std::string& text, unsigned int type); //Creation of the shader



	~Shader(); //Destructor


protected:
private:
	static const unsigned int NUM_SHADS = 2; // number of shaders
	
	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	
	//Shader(const Shader& other) {}
	//void operator=(const Shader& other) {}

	GLuint program; // Track the shader program
	GLuint shaders[NUM_SHADS]; //array of shaders
	GLuint uniforms[NUM_UNIFORMS];//number of uniform variables
};

