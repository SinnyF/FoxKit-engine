
#include "headers\Renderer.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

	static unsigned int VBO, VAO;
	static unsigned int EBO;
	static vector<Shader> shaders;


	static float tempCols[9] = {0,0,0,0,0,0,0,0,0};

	GLenum glCheckError_(const char* file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
	
void WindowResized(GLFWwindow* window, int width, int height) 
{
	glViewport(0,0,width,height);
}

int Renderer::InitializeRenderer()
{
	cout << "Initializing Renderer..." << endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1366, 768, "FoxKit engine", NULL, NULL);
	

	if (window == NULL) {
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return 0;
	}

	glViewport(0, 0, 1366, 768);
	glfwSetFramebufferSizeCallback(window, WindowResized);

	float triangle[18] = {
		0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glCheckError();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), &triangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glCheckError();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glCheckError();
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	glCheckError();

	shaders.push_back( Shader("src/shaders/base.vert", "src/shaders/base.frag"));
	shaders.push_back(Shader("src/shaders/base.vert", "src/shaders/base2.frag"));
	
	SetColors();

	return 1;
}

void Renderer::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	int colorLocation1 = glGetUniformLocation(shaders[0].ID, "color1");
	int colorLocation2 = glGetUniformLocation(shaders[0].ID, "color2");
	int colorLocation3 = glGetUniformLocation(shaders[0].ID, "color3");
	int colSolidLocation = glGetUniformLocation(shaders[1].ID, "colSolid");
	shaders[0].use();
	glUniform3f(colorLocation1, tempCols[0], tempCols[1], tempCols[2]);
	glUniform3f(colorLocation2, tempCols[3], tempCols[4], tempCols[5]);
	glUniform3f(colorLocation3, tempCols[6], tempCols[7], tempCols[8]);
	DrawTriangle(0,0.0f, -0.1f, 0.0f, 0.5f);
	DrawTriangle(0, 0.0f, -0.1f, 0.0f, 0.5f, -1.0f);
	shaders[1].use();

	glUniform3f(colSolidLocation, 0.0f, 1.0f, 0.0f);
	DrawRectangle(1, -0.4f, -0.9f, 0.0f, 0.8f);
	glUniform3f(colSolidLocation, 1.0f, 0.0f, 0.0f);
	DrawRectangle(1,0.1f, -0.9f, 0.0f, 0.3f);
	glUniform3f(colSolidLocation, 0.0f, 0.0f, 1.0f);
	DrawRectangle(1,-0.4f, -0.4f, 0.0f, 0.2f);
	DrawRectangle(1,0.2f, -0.4f, 0.0f, 0.2f);

	glCheckError();

	glfwSwapBuffers(window);
}

void Renderer::Terminate() {
	glfwTerminate();
}

GLFWwindow* Renderer::GetWindow()
{
	return window;
}

void Renderer::SetColors()
{
	srand(time(0));
	
	for (int i = 0; i < 9; i++) {
		tempCols[i] = (float)(rand() % 101) / 100.0f;
	}
	
}

void Renderer::DrawTriangle(int shader, float offsetX, float offsetY, float offsetZ, float scale, float flipX, float flipY, float flipZ)
{
	int uniform1 = glGetUniformLocation(shaders[shader].ID, "offset");
	int uniform2 = glGetUniformLocation(shaders[shader].ID, "scaling");
	int uniform3 = glGetUniformLocation(shaders[shader].ID, "flip");
	
	glUniform3f(uniform1, offsetX, offsetY, offsetZ);
	glUniform3f(uniform3, flipX, flipY, flipZ);
	glUniform1f(uniform2, scale);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glCheckError();
}

void Renderer::DrawRectangle(int shader, float offsetX, float offsetY, float offsetZ, float scale)
{
	DrawTriangle(shader, offsetX, offsetY, offsetZ, scale);
	DrawTriangle(shader, offsetX + scale, offsetY + scale, offsetZ , scale, -1.0f, -1.0f);
}

