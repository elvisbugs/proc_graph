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
		objOpenGl.run(dblColour[0], dblColour[1], dblColour[2], 1);

		objOpenGl.delay(20);
		glfwGetTime();
	}
	objOpenGl.terminate();
}