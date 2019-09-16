#pragma once
#include "iostream"
#include "InterfaceOGL.h"
#include <stdlib.h>
#include "MyScene.h"
#include <memory>
#include "ReadShaders.h"

using namespace std;

int main() {
	InterfaceOGL objOpenGl;

	double dblColour[3] = { 0.1,0.1,0.1 };

	string vertexShaderSource;
	string fragmentShaderSource;

	loadFile("Shaders\\Vertex.vs", vertexShaderSource);
	loadFile("Shaders\\Fragment.fs", fragmentShaderSource);

	const char* vertex_shader_source = vertexShaderSource.c_str();
	const char* fragment_shader_source = fragmentShaderSource.c_str();

	objOpenGl.setScene(unique_ptr<Scene>(new MyScene(vertex_shader_source, fragment_shader_source)));

	while (!objOpenGl.needCloseWindow()) {
		for (int i = 0; i < 3; i++) {
			dblColour[i] = ((double)rand() / (RAND_MAX));
		}

		/*GLfloat colors[4];

		if (objOpenGl.processInput(GLFW_KEY_1))
		{
			colors[0] = (float)sin(objOpenGl.currentTime() / 1000);
			colors[1] = (float)cos(objOpenGl.currentTime() / 1000);
			colors[2] = 0;
			colors[3] = 1;
		}*/

		/*objOpenGl.run(colors);*/

		objOpenGl.run(dblColour[0], dblColour[1], dblColour[2], 1);

		objOpenGl.delay(100);
	}
	objOpenGl.terminate();
}