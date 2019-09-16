#pragma once
#include "Scene.h"
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

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

	float x = 0, y = -1;
	int incY = 1, incX = 0;
};

