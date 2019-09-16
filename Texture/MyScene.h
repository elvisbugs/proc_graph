#pragma once

#include "Scene.h"
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <vector>

using namespace std;

class MyScene : public Scene
{
public:
	MyScene(const char* vertex_shader_source, const char* fragment_shader_source);

	~MyScene();

	void run();

	void generateVBO();

	GLuint getProgram() { return mProgram; }

private:
	void generateProgram(GLuint &program);
	void generateShaders(const char* vertex_shader_source, const char* fragment_shader_source);

	GLuint mVertexShader = 0;
	GLuint mFragmentShader = 0;

	GLuint mProgram = 0;

	unsigned int texture1, texture2;;

	GLuint mIntTextureID;

	unsigned int VBO, VAO, EBO;

	float x, y;

	int mIntSuccess;
	char mChrInfoLog[512];

	void Textures(const char* pStrFilePath);
};

