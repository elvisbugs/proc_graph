#include "MyScene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

MyScene::~MyScene()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}

MyScene::MyScene(const char* vertex_shader_source, const char* fragment_shader_source)
{
	this->generateShaders(vertex_shader_source, fragment_shader_source);
	this->generateProgram(this->mProgram);
	this->generateVBO();

	this->Textures("Textures\\container.jpg");
}

void MyScene::Textures(const char* pStrFilePath)
{
	// load and create a texture 
	// -------------------------

	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(pStrFilePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//// texture 2
	//// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("Textures\\awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glUseProgram(this->mProgram);
	glUniform1i(glGetUniformLocation(this->mProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(this->mProgram, "texture2"), 1);
}

void MyScene::generateVBO()
{
	//cria os vertices do triangulo
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void MyScene::run()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

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

	//// create transformations
	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(this->x, this->y, 0.0f));
	transform = glm::scale(transform, glm::vec3(0.1f, 0.1f, 0.0f));

	// get matrix's uniform location and set matrix
	glUseProgram(this->mProgram);
	unsigned int transformLoc = glGetUniformLocation(this->mProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// render container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void MyScene::generateProgram(GLuint& program)
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