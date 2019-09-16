#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string.h>
#include <time.h>
#include <memory>

class Scene;

using namespace std;

class InterfaceOGL
{
public:
	InterfaceOGL(int Witdh, int Heigth, char * title);
	InterfaceOGL(char* title);
	InterfaceOGL();

	~InterfaceOGL();

	inline void setScene(unique_ptr<Scene> scene) { m_Scene = move(scene); }

	void run(float red, float green, float blue, float opacidade);

	void run(GLfloat* colors);

	bool needCloseWindow();

	void processInput();

	bool processInput(int key);

	void terminate();

	void delay(int milliSeconds);

	long currentTime();

	GLFWwindow* getWindow() {return this-> window;}

protected:
	int width = 1240;
	int height = 600;

	unique_ptr <Scene> m_Scene = nullptr;
	
	long time;

	void initOpenGL();

	GLFWwindow* window = NULL;

	static void resizeWindow(GLFWwindow* window,int width, int height);
	
	void StartGlfw(int Width, int Heigth, char* title);
};