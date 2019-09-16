#include "MyScene.h"
#include <glad/glad.h>
#include <iostream>

using namespace std;

MyScene::MyScene() 
{

}

MyScene::~MyScene()
{
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
}

MyScene::MyScene(const char* vertex_shader_source, const char* fragment_shader_source)
{
	this->generateShaders(vertex_shader_source, fragment_shader_source);
	this->generateProgram(this->mProgram);
	this->generateVBO();
}

void MyScene::generateVBO()
{
	//cria os vertices do triangulo
	float vertices[] = {
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	glGenBuffers(1, &this->vbo);
	glGenVertexArrays(1, &this->vao);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 
		3, 
		GL_FLOAT,
		GL_FALSE, 
		6 * sizeof(float), 
		(GLvoid *)0);

	glVertexAttribPointer(1,
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		6 * sizeof(float), 
		(GLvoid *) (3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void MyScene::run()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(this->mProgram);
	glBindVertexArray(this->vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glBindVertexArray(0);
}

void MyScene::generateShaders(const char* vertex_shader_source, const char* fragment_shader_source)
{
	//cria e compila um vertex shader
	this->mVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->mVertexShader, 1, &vertex_shader_source, NULL);
	glCompileShader(this->mVertexShader);

	glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &this->mIntSuccess);
	if (!this->mIntSuccess) {
		glGetShaderInfoLog(mVertexShader, 512, NULL, this->mChrInfoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << this->mChrInfoLog << endl;
	}

	//cria e compila um fragment shader
	this->mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->mFragmentShader, 1, &fragment_shader_source, NULL);
	glCompileShader(this->mFragmentShader);

	glGetShaderiv(this->mFragmentShader, GL_COMPILE_STATUS, &this->mIntSuccess);
	if (!this->mIntSuccess)
	{
		glGetShaderInfoLog(this->mFragmentShader, 512, NULL, this->mChrInfoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << this->mChrInfoLog << endl;
	}
}

void MyScene::generateProgram(GLuint &program)
{
	//cria um program, vincula o vertex shader e o fragment shader e linka com o programa
	
	program = glCreateProgram();
	glAttachShader(program, this->mVertexShader);
	glAttachShader(program, this->mFragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &this->mIntSuccess);
	if (!this->mIntSuccess) {
		glGetProgramInfoLog(program, 512, NULL, this->mChrInfoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << this->mChrInfoLog << endl;
	}

	//deleta os shaders
	glDeleteShader(this->mVertexShader);
	glDeleteShader(this->mFragmentShader);
}