#include "System.h"
#include "Rectangle.h"
using namespace std;
System::System()
{
	srand(clock());
}

System::~System()
{
	delete shaderMap;
}

void System:: mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		cout << "Cursor Position at (" << xpos << " : " << 600 - ypos << endl;
		
	}
}

int System::GLFWInit()
{

	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	glfwWindowHint( GLFW_SAMPLES, 4 );

	window = glfwCreateWindow( WIDTH, HEIGHT, "Sabertooth", nullptr, nullptr );

	glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

	if ( window == nullptr ) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );

	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK ) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport( 0, 0, screenWidth, screenHeight );

	glfwSetMouseButtonCallback(this->window, &mouse_button_callback);

	return EXIT_SUCCESS;

}

int System::OpenGLSetup()
{

	glEnable( GL_BLEND );	// Enables blending ( glBlendFunc )
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_DEPTH_TEST );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CW );

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	shaderMap = new vector<Shader*>();
	for (int i = 0; i < 800; i++)
	{
		shaderMap->push_back(new Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", i));
		shaderMap->at(i)->Use();
	}
	//coreShader = Shader( "Shaders/Core/core.vert", "Shaders/Core/core.frag" );
	//coreShader.Use();

	return EXIT_SUCCESS;
}

void System::Run()
{
	GLuint VBO[800], VAO[800], EBO[800];
	int k = 0;
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			shaderMap->at(k)->Use();
			//coreShader.Use();
			GLfloat width = 40;
			GLfloat height = 14.63415;
			GLfloat r = rndFloat() ,g = rndFloat() ,b = rndFloat();

			Rectangle* objRect = new Rectangle(38, 12.68293, j * width, (i+1) * height, WIDTH, HEIGHT, r, g, b);
			shaderMap->at(k)->setRGB(r, g, b);
			glGenVertexArrays(1, &VAO[k]);
			glGenBuffers(1, &VBO[k]);
			glGenBuffers(1, &EBO[k]);

			// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
			glBindVertexArray(VAO[k]);

			glBindBuffer(GL_ARRAY_BUFFER, VBO[k]);
			glBufferData(GL_ARRAY_BUFFER, objRect->getVertSize(), objRect->getVertices(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[k]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, objRect->getIndSize(), objRect->getIndices(), GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			// Texture attribute
			//glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) ( 12 * sizeof( GLfloat ) ) );
			//glEnableVertexAttribArray( 1 );

			glBindVertexArray(0); // Unbind VAO
			k++;
		}
	}
	
	while ( !glfwWindowShouldClose( window ) ) {

		glfwPollEvents();

#pragma region Input Handling

		if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
			glfwSetWindowShouldClose( window, GLFW_TRUE );
		}

#pragma endregion

		glClearColor( 0, 0, 0, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		for (int i = 0; i < 800; i++)
		{
			shaderMap->at(i)->Use();

			if (i % 4 == 0)
			{
				glUniform1i(glGetUniformLocation(this->shaderMap->at(i)->program, "rectOn"), false);
			}
			glBindVertexArray(VAO[i]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
		}
		glBindVertexArray(0);
		
		glfwSwapBuffers( window );
	}


}

void System::Finish()
{
	for (int i = 0; i < 800; i++)
	{
		shaderMap->at(i)->Delete();
	}

	glfwTerminate();
}
