#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <string>
#include <iostream>

using namespace std;

class Textures
{
public:
	Textures(string pStrFilePath);
	~Textures();
	
	unsigned int getTextureID() { return this->mIntTextureID; }

private:
	unsigned int mIntTextureID;
};