#ifndef SYSTEM_H
#define SYSTEM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Time.h"
#include <vector>
using namespace std;
class System
{
private:
	// Screen
	const GLfloat WIDTH = 800, HEIGHT = 600;
	int screenWidth, screenHeight;
	int wtf;

	float rndFloat()
	{
		float r3 = ((float)rand()) / RAND_MAX;
				
		return r3;
	}

public:
	GLFWwindow* window;
	vector<Shader*> *shaderMap;

	void setWtf(int set) { wtf = set; }

public:
	System();
	~System();

	int GLFWInit();
	int OpenGLSetup();
	int SystemSetup();

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	void Run();

	void Finish();

};

#endif