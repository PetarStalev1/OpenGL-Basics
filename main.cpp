#include <stdio.h>
#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


 //Windows dimension
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;
// Two kinds of shaders
// 
// Vertex Shader
// Fragment shader handing each point on the screen

static const char* vShader = "								\n\
#version 330												\n\
layout(location = 0) in vec3 pos;							\n\
void main()													\n\
{															\n\
	gl_Position = vec4(0.4 * pos,1.0);	\n\
															\n\
															\n\
}															\n\
";

static const char* fShader = "								\n\
#version 330												\n\
out	vec4 colour;											\n\
void main()													\n\
{															\n\
	colour = vec4(1.0,	0.0,	0.0,	1.0);				\n\
															\n\
															\n\
}															\n\
";

// in - input value

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};
	// creating a vertex array on the GPU, defining spcae on the memory
	// givingg the ID of the vertex array
	// VAO - Vertex Array Object
	glGenVertexArrays(1, &VAO);
	// Any OPENGL functions we use will interact with the VAO
	glBindVertexArray(VAO);
 
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// basically we are saying  0 because everything is going to location = 0
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


	// In here we are basically saying to unbind the VAO so we are undoing the command: glBindVertexArray(VAO);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode,GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;


	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	// Place to log the error
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: %s\n", shaderType,eLog);
		return;
	}

	glAttachShader(theProgram, theShader);

	return;
}
void CompileShaders()
{
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creating shader program\n");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);
	

	// The result of the functions we are going to perform
	GLint result = 0;
	// Place to log the error
	GLchar eLog[1024] = { 0 };

	
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS,&result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s\n", eLog);
		return;
	}

	// checking if the current shader we are working with is 
	// valid in the current context which OpenGL is working in
	glValidateProgram(shader);

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s\n", eLog);
		return;
	}
}

int main()
{

	
	// Initializer GLFW
	// Should return GLEW_OK
	// Can read the error using glewGetErrorString(result);
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	// SetUp GLFW windows properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards compatability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "TEST", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);


	// Set context for GLFW to use
	// use the window we have just created and draw the context to that
	glfwMakeContextCurrent(mainWindow);


	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("glewInit initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}


	// Setup ViewPort Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// After we have compiled the shaders
		// It will grab the ID we created
		// it will go to the GPU and it will say:
		// "Use the one with the Id of "shader" ;) "
		glUseProgram(shader);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		// unassigning the shader
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}
	return 0;
}