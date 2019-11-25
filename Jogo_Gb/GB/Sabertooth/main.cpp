
#include <GL/glew.h>																	/* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <SOIL.h>
#include <time.h>
#include <string>
#include "stb_image.h"
#include <vector>
#include <list> 

using namespace std;

float wWidth = 1200.0f, wHeight = 1000.0f;
float qtdX = 11.0;
float qtdY = qtdX * 2;
float RW = wWidth / qtdX;
float RH = wHeight / qtdY;

float vertices[] = {
	// positions		//texture
	 0.0f, RH / 2.0f, 0.0f,			   0.0f, 0.5f,  // top right
	 RW / 2.0f,   RH,	 0.0f,	 (1.f / 36), 1.0f,	// bottom right
	 RW,   RH / 2.0f, 0.0f,	 (1.f / 18), 0.5f, // bottom left
	 RW / 2.0f, 0.0F, 0.0f,	 (1.f / 36), 0.0f, // top left 
};
GLuint indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

void loadImage(char *path) {
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);   // PASSA A IMAGEM PARA UMA VARIÁVEL
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);   // PASSA TEXTURA PRA DENTRO DO PROGRAMA
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

GLuint generateShader(float wWidth, float wHeight, GLuint &VAO, GLuint &VBO, GLuint &EBO, GLuint &texture) {
	const char* vertex_shader =
		"#version 410\n"
		"layout(location = 0) in vec3 vp;"
		"layout(location = 1) in vec2 texture_mapping;"

		"out vec2 texture_coords;"

		"uniform mat4 proj;"
		"uniform mat4 trans;"

		"void main () {"
		"texture_coords = texture_mapping;"
		" gl_Position = proj * trans * vec4 (vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 410\n"

		"in vec2 texture_coords;"

		"out vec4 frag_color;"

		"uniform sampler2D sprite;"
		"uniform vec3 color;"
		"uniform vec3 light;"
		"uniform float offsetx;"
		"uniform float offsety;"

		"void main () {"
		"vec4 texel = texture(sprite, vec2(texture_coords.x + offsetx, texture_coords.y + offsety));"
		"if(texel.a < 1) discard;"
		"frag_color = texel;"
		"}";

	// identifica vs e o associa com vertex_shader
	

	VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	// identifica fs e o associa com fragment_shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glBindFragDataLocation(shader_programme, 0, "frag_color");
	glLinkProgram(shader_programme);
	glUseProgram(shader_programme);											// passagem de variáveis CPU para GPU

	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	
	loadImage("tree.png");
	loadImage("tiles2.png");
	return shader_programme;
}

void delay(int milliSeconds)
{
	clock_t start_time = clock();
	while (clock() < start_time + milliSeconds);
};

int main() {
		
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	
	GLFWwindow *window = glfwCreateWindow(
		1200.0f, 1000.0f, "Saboteur", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);   // inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint VBO, VAO, EBO;
	GLuint texture;
	GLuint shdr = generateShader(wWidth, wHeight, VAO, VBO, EBO, texture);;
	
	time_t t;
	int y = 0;
	int x = 0;

	float tileMap[11][11];
	int tabuleiro[11][11];
	int possivel[11][11];
	srand(time(NULL));
	for (int y = 0; y < qtdX; y++) {
		for (int x = 0; x < qtdX; x++)
		{
			tileMap[y][x] = 11;
			tabuleiro[y][x] = 0;
			possivel[y][x] = 0;
		}
	}
	possivel[1][4] = 1;
	possivel[1][6] = 1;
	possivel[0][5] = 1;
	possivel[2][5] = 1;

	tileMap[1][5] = 0;
	tileMap[9][5] = 12;
	tileMap[9][2] = 12;
	tileMap[9][8] = 12;

	tabuleiro[1][5] = 1;
	tabuleiro[9][5] = 1;
	tabuleiro[9][2] = 1;
	tabuleiro[9][8] = 1;

	

	double xpos = 0;
	double ypos = 0;

	double nyPos = 0;
	double nxPos = 0;

	bool mousePressed = true; //para renderizar a primeira vez
		   	 
	glViewport(0.0f, 0.0f, wWidth, wHeight);
	glm::mat4 projection = glm::ortho(0.0f, wWidth, wHeight, 0.0f, -1.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {

		// Define vao como verte array atual
		glClearColor(0.34f, 0.49f, 0.27f, 0.0f);
		//glClearColor(0, 0, 0, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso

		glUniformMatrix4fv(glGetUniformLocation(shdr, "proj"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		glUniform3f(glGetUniformLocation(shdr, "light"), 1.0f, 1.0f, 1.0f);


		//inicio rodada "for"
			
			for (int y = 0; y < qtdX; y++) {
				for (int x = 0; x < qtdX; x++) {

					float xi = x * (RW / 2.0f) + y * (RW / 2.0f);
					float yi = (x * (RH / 2.0f) - y * (RH / 2.0f) + wHeight / 2.0f);

					glm::mat4 transform = glm::mat4(1.0f);
					transform = glm::translate(transform, glm::vec3(xi, yi, 0.0f));
					glUniformMatrix4fv(glGetUniformLocation(shdr, "trans"), 1, GL_FALSE, glm::value_ptr(transform));
									   
					float var = tileMap[y][x] * (1.0f / 18.0f);
					glUniform1f(glGetUniformLocation(shdr, "offsetx"), var);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texture);

					glUniform1i(glGetUniformLocation(shdr, "sprite"), 0);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					xpos = 0;
					ypos = 0;

					nyPos = 0;
					nxPos = 0;
				}
			}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			for (int y = 0; y < qtdX; y++) {
				for (int x = 0; x < qtdX; x++)
				{
					tileMap[y][x] = 11;
					tabuleiro[y][x] = 0;
					possivel[y][x] = 0;
				}
			}
			possivel[1][4] = 1;
			possivel[1][6] = 1;
			possivel[0][5] = 1;
			possivel[2][5] = 1;

			tileMap[1][5] = 0;
			tileMap[9][5] = 12;
			tileMap[9][2] = 12;
			tileMap[9][8] = 12;

			tabuleiro[1][5] = 1;
			tabuleiro[9][5] = 1;
			tabuleiro[9][2] = 1;
			tabuleiro[9][8] = 1;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			//getting cursor position
			
			glfwGetCursorPos(window, &xpos, &ypos);

			int mouseLinha = -1 * (11 - (((xpos / (RW / 2.0f)) + (ypos / (RH / 2.0F))) / 2));
			int mouseColuna = 11 - (-(xpos / (RW / 2.0f)) + (ypos / (RH / 2.0F))) / 2;

			bool tes1 = (mouseColuna == 9 && mouseLinha == 5);
			bool tes2 = (mouseColuna == 9 && mouseLinha == 2);
			bool tes3 = (mouseColuna == 9 && mouseLinha == 8);


			if (!tes1 && !tes2 && !tes3 && tabuleiro[mouseColuna][mouseLinha] == 0 && possivel[mouseColuna][mouseLinha] == 1) {
				int card = rand() % 7;
				tileMap[mouseColuna][mouseLinha] = card;
				tabuleiro[mouseColuna][mouseLinha] = 1;
				//card inicial
				if (card == 0) {
					possivel[mouseColuna][mouseLinha - 1] = 1;
					possivel[mouseColuna][mouseLinha + 1] = 1;
					possivel[mouseColuna + 1][mouseLinha] = 1;
					possivel[mouseColuna - 1][mouseLinha] = 1;
				}
				//car reto y 
				else if(card == 1) {
					possivel[mouseColuna-1][mouseLinha] = 1;
					possivel[mouseColuna+1][mouseLinha] = 1;

					possivel[mouseColuna][mouseLinha - 1] = 0;
					possivel[mouseColuna][mouseLinha + 1] = 0;
				}
				//card vira a direita
				else if (card == 2) {
					possivel[mouseColuna][mouseLinha+1] = 1;
					possivel[mouseColuna - 1][mouseLinha] = 1;

					possivel[mouseColuna + 1][mouseLinha] = 0;
					possivel[mouseColuna][mouseLinha - 1] = 0;
				}
				//card em t para os lados
				else if (card == 3) {
					possivel[mouseColuna][mouseLinha + 1] = 1;
					possivel[mouseColuna][mouseLinha - 1] = 1;
					possivel[mouseColuna-1][mouseLinha] = 1;

					possivel[mouseColuna + 1][mouseLinha] = 0;
				}
				//card em t em y
				else if (card == 4) {
					possivel[mouseColuna][mouseLinha + 1] = 1;
					possivel[mouseColuna][mouseLinha - 1] = 1;
					possivel[mouseColuna - 1][mouseLinha] = 1;
					possivel[mouseColuna + 1][mouseLinha] = 1;

					possivel[mouseColuna][mouseLinha - 1] = 0;
				}
				//card tranca caminho em x
				else if (card == 5) {
					possivel[mouseColuna][mouseLinha + 1] = 0;
					possivel[mouseColuna][mouseLinha - 1] = 0;
				}
				//card tranca caminho em y
				else if (card == 6) {
					possivel[mouseColuna - 1][mouseLinha] = 0;
					possivel[mouseColuna + 1][mouseLinha] = 0;
				}
				//car reto x
				else if (card == 7) {
					possivel[mouseColuna - 1][mouseLinha] = 0;
					possivel[mouseColuna + 1][mouseLinha] = 0;

					possivel[mouseColuna][mouseLinha - 1] = 1;
					possivel[mouseColuna][mouseLinha + 1] = 1;
				}
				//vira pra cima
				else if (card == 8) {
					possivel[mouseColuna][mouseLinha + 1] = 0;
					possivel[mouseColuna - 1][mouseLinha] = 0;

					possivel[mouseColuna + 1][mouseLinha] = 1;
					possivel[mouseColuna][mouseLinha - 1] = 1;
				}
				//vira pra cima
				else if (card == 9) {
					possivel[mouseColuna][mouseLinha + 1] = 1;
					possivel[mouseColuna - 1][mouseLinha] = 0;

					possivel[mouseColuna + 1][mouseLinha] = 1;
					possivel[mouseColuna][mouseLinha - 1] = 0;
				}
				//vira pra cima
				else if (card == 10) {
					possivel[mouseColuna][mouseLinha + 1] = 0;
					possivel[mouseColuna - 1][mouseLinha] = 1;

					possivel[mouseColuna + 1][mouseLinha] = 0;
					possivel[mouseColuna][mouseLinha - 1] = 1;
				}
				if (mouseLinha == 5 && mouseColuna == 8) {
					int var = (1 + rand() % 2 + 12);
					tileMap[9][5] = var;
				}
				if (mouseLinha == 2 && mouseColuna == 8) {
					int var = (1 + rand() % 2 + 12);
					tileMap[9][2] = var;
				}
				if (mouseLinha == 8 && mouseColuna == 8) {
					int var = (1 + rand() % 2 + 12);
					tileMap[9][8] = var;
				}
			}

			std::cout << "linha: " << mouseLinha << " coluna: " << mouseColuna << std::endl;

			delay(250);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}