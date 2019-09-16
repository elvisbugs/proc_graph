#include "InterfaceOGL.h"

using namespace std;

void InterfaceOGL::StartGlfw(int Width, int Heigth, char* title) 
{
	time = clock();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(this->width, this->height, title, NULL, NULL);

	this->initOpenGL();
}

InterfaceOGL::InterfaceOGL(int Width, int Heigth, char *title)
{
	StartGlfw(Width, Heigth, title);
}

InterfaceOGL::InterfaceOGL()
{
	char title[8] = "Default";
	StartGlfw(this->width, this->height, title);
}

InterfaceOGL::InterfaceOGL(char * title)
{
	StartGlfw(this->width, this->height, title);
}

InterfaceOGL::~InterfaceOGL()
{
	this->terminate();
}

void InterfaceOGL::terminate()
{
	glfwTerminate();
}

void InterfaceOGL::resizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void InterfaceOGL::initOpenGL()
{
	glfwMakeContextCurrent(window);

	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return;
	}

	glfwSetFramebufferSizeCallback(window, InterfaceOGL::resizeWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glViewport(0, 0, this->width, this->height);
}

void InterfaceOGL::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

bool InterfaceOGL::processInput(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool InterfaceOGL::needCloseWindow()
{
	return glfwWindowShouldClose(window);
}

void InterfaceOGL::run(float red, float green, float blue, float transparencia)
{
		this->processInput();

		glClearColor(red, green, blue, transparencia);
		glClear(GL_COLOR_BUFFER_BIT);

		if (m_Scene)
			m_Scene->run();

		glfwSwapBuffers(this->window);
		glfwPollEvents();
}

void InterfaceOGL::run(GLfloat * colors)
{
	this->processInput();

	glClearBufferfv(GL_COLOR, 0, colors);

	if (m_Scene)
		m_Scene->run();

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void InterfaceOGL::delay(int milliSeconds)
{
	clock_t start_time = clock();
	while (clock() < start_time + milliSeconds);
};

long InterfaceOGL::currentTime()
{
	return clock() - time;
}