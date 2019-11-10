#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;
class Rectangle
{
public:
	//width, height = largura e altura do retangulo
	//xBtmLeft, yBtmLeft = ponto inferior a esquerda do triangulo (ponto inicial)
	//windowWidth e windowHeigth = tamanho da janela onde será desenhado
	//r,g,b = cores do retangulo
	Rectangle(GLfloat width, GLfloat height, GLfloat xBtmLeft, GLfloat yBtmLeft, GLfloat windowWidth, GLfloat windowHeight, GLfloat r, GLfloat g, GLfloat b);
	~Rectangle() {
		vertices = NULL;
		delete  vertices;
		indices = NULL;
		delete indices;
	}
	GLfloat* getVertices();

	GLuint* getIndices();

	GLint getVertSize() { return vertSize; }

	GLint getIndSize() { return indSize; }

private:
	GLfloat  * vertices = new GLfloat[24];//(GLfloat*)calloc(24, sizeof(GLfloat));
	GLuint * indices = new GLuint[6];//(GLuint*)calloc(6, sizeof(GLuint));

	GLint vertSize = 0;
	GLint indSize = 0;
};

