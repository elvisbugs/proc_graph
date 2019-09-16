#pragma once
#include "Scene.h"
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

class MyScene : public Scene
{
public:
	MyScene();
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

	unsigned int vbo, vao;

	int mIntSuccess;
	char mChrInfoLog[512];
};

