
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

#include "stb_image.h"

unsigned char * loadImage(char *path) {
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

	return data;
}


int main() {

	srand(time(NULL));
	
    /*(rand() % 255) / 255.f;
    (rand() % 255) / 255.f;
    (rand() % 255) / 255.f;
	*/

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

	/////////////////////////////////////////////////////////////////////////////////////////
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

	float qtdX = 11.0;
	float qtdY = qtdX * 2;
	float RW = 1200.0f / qtdX;
	float RH = 1000.0f / qtdY;

	// identifica vs e o associa com vertex_shader
	float vertices[] = {
		// positions		//texture
		 0.0f, RH / 2.0f-20, 0.0f,			   0.0f, 0.5f,  // top right
		 RW / 2.0f,   RH-20,	 0.0f,	 (1.f / 36), 1.0f,	// bottom right
		 RW,   RH / 2.0f-20, 0.0f,	 (1.f / 18), 0.5f, // bottom left
		 RW / 2.0f, 0.0F-20, 0.0f,	 (1.f / 36), 0.0f, // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
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

	// TEXTURA
	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char* textura = loadImage("tiles2.png");

	time_t t;
	int y = 0;
	int x = 0;

	int nxPos = 0;
	int nyPos = 0;
	int nTotal = 0;

	double xpos, ypos;

	float tileMap[11][11];
	srand(time(NULL));
	for (int y = 0; y < qtdX; y++) {
		for (int x = 0; x < qtdX; x++)
		{
			tileMap[y][x] = 14;
		}
	}
	tileMap[1][5] = 0;
	tileMap[9][5] = 8;
	tileMap[9][2] = 8;
	tileMap[9][8] = 8;
		   	 
	glViewport(0.0f, 0.0f, 1200.0f, 1000.0f);
	glm::mat4 projection = glm::ortho(0.0f, 1200.0f, 1000.0f, 0.0f, -1.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {

		// Define vao como verte array atual
		glClearColor(0.34f, 0.49f, 0.27f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso

		glUniformMatrix4fv(glGetUniformLocation(shader_programme, "proj"), 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(VAO);

		glUniform3f(glGetUniformLocation(shader_programme, "light"), 1.0f, 1.0f, 1.0f);


		//inicio rodada "for"

		for (y = 0; y < qtdX; y++) {

			for (x = 0; x < qtdX; x++) {

				glm::mat4 transform = glm::mat4(1.0f);
				transform = glm::translate(transform, glm::vec3((x*(RW / 2.0f) + y * (RW / 2.0f)),((x*(RH / 2.0f) - y * (RH / 2.0f) + 250.0f)), 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_programme, "trans"), 1, GL_FALSE, glm::value_ptr(transform));

				//int i = x + y * 20;
				float var = tileMap[y][x] * (1.0f / 18.0f);
				glUniform1f(glGetUniformLocation(shader_programme, "offsetx"), var);
				//glUniform1f(glGetUniformLocation(shader_programme, "offsety"), 14);


				/*Exemplo:
					ID = identificador único / endereço do tile
						coluna tile = ID % num de colunas de tiles
						linha tile = ID / num de linhas de tiles
						ID = 43
						col = 43 % 10 = 3;
					row = 43 / 10 = 4;
					offsetx = 3 / 10 = 0.3;
					offsety = 4 / 10 = 0.4;*/


				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);

				glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);


				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}

		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		//click do mouse

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			//getting cursor position
			glfwGetCursorPos(window, &xpos, &ypos);
			nxPos = (xpos - nyPos * RW / 2) / (RW / 2);
			nyPos = (nxPos*RH / 2 - ypos) / (RH / 2);

			nTotal = nxPos + nyPos * 20;

			std::cout << "x: " << nxPos << " e y: " << nyPos << " e o losangulo: " << nTotal << std::endl;
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