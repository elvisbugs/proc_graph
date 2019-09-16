#include "Textures.h"

Textures::Textures(string pStrFilePath)
{
	glGenTextures(1, &this->mIntTextureID);
	glBindTexture(GL_TEXTURE_2D, this->mIntTextureID);

	int intWidth, intHeight, intNrChannels;
	unsigned char* ucharData = stbi_load(pStrFilePath.c_str(), &intWidth, &intHeight, &intNrChannels, 0);

	if (ucharData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, intWidth, intHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucharData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, intWidth, intHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucharData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(ucharData);
}

Textures::~Textures()
{
}