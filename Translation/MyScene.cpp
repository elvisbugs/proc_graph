#include "MyScene.h"

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *) (3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void MyScene::run()
{
	glClear(GL_COLOR_BUFFER_BIT );

	// update the uniform color
	float timeValue = glfwGetTime();
	float greenValue = sin(timeValue) / 2.0f + 0.5f;

	if (this->incX == 0)
	{
		this->x += 0.1;
		if (this->x >= 1)
			this->incX = 1;
	}
	else
	{
		this->x -= 0.1;
		if (this->x <= -1)
			this->incX = 0;
	}

	if (this->incY == 0)
	{
		this->y += 0.1;
		if (this->y >= 1)
			this->incY = 1;
	}
	else
	{
		this->y -= 0.1;
		if (this->y <= -1)
			this->incY = 0;
	}

	// create transformations
	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(this->x, this->y, 0.0f));
	//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	//transform = glm::scale(transform, glm::vec3(sin(timeValue)*2, sin(timeValue)*2, sin(timeValue)*2));
	
	int vertexColorLocation = glGetUniformLocation(this->mProgram, "ourColor");

	glUseProgram(this->mProgram);

	unsigned int transformLoc = glGetUniformLocation(this->mProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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