#include "Rectangle.h"

Rectangle::Rectangle(GLfloat width, GLfloat height, GLfloat xBtmLeft, GLfloat yBtmLeft, GLfloat windowWidth, GLfloat windowHeight, GLfloat r, GLfloat g, GLfloat b)
{
	GLfloat xNormalized = (xBtmLeft / (windowWidth/2)) - 1;
	GLfloat yNormalized = (yBtmLeft / (windowHeight/2)) - 1;
	width = (width / (windowWidth/2));
	height = (height / (windowHeight/2));

	GLfloat vert[24] =
	{
		xNormalized + width, yNormalized + height, 0, r, g, b,
		xNormalized + width, yNormalized,		   0, r, g, b,
		xNormalized,         yNormalized,		   0, r, g, b,
		xNormalized,		 yNormalized + height, 0, r, g, b,
	};

	GLuint ind[6] = { 0, 1, 3, 1, 2, 3 };

	this->vertSize = sizeof(vert);
	this->indSize = sizeof(ind);

	for (int i = 0; i < 24; i++)
	{
		this->vertices[i] = vert[i];
	}
	for (int i = 0; i < 6; i++)
	{
		this->indices[i] = ind[i];
	}
}

GLfloat* Rectangle::getVertices() 
{ 
	return vertices;
}

GLuint* Rectangle::getIndices()
{ 
	return indices;
}