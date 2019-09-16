#pragma once
#include "include/glad/glad.h"
#include "GLFW/glfw3.h"
#include <string.h>
#include <time.h>

using namespace std;

class InterfaceOGL
{
public:
	InterfaceOGL(int Witdh, int Heigth, char * title);
	InterfaceOGL(char* title);
	InterfaceOGL();

	~InterfaceOGL();

	void run(float red, float green, float blue, float opacidade);

	void run(GLfloat* colors);

	bool needCloseWindow();

	void processInput();

	bool processInput(int key);

	void terminate();

	void delay(int milliSeconds);

	long currentTime();

protected:
	int width = 1240;
	int height = 600;
	
	long time;

	void initOpenGL();

	GLFWwindow* window = NULL;

	void resizeWindow(GLFWwindow* window,int width, int height);
	
	void StartGlfw(int Width, int Heigth, char* title);
};