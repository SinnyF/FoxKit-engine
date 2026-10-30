
#include "headers\Renderer.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

	static unsigned int VBO, VAO;
	static unsigned int EBO;
	static vector<Shader> shaders;
	
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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	shaders.push_back( Shader("src/shaders/base.vert", "src/shaders/base.frag"));
	shaders[0].use();
	SetColors();

	return 1;
}

void Renderer::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	

	vector<float> rect = {
		-0.5f,-0.7f, 0.0f, 0.0f, 0.0f, 1.0f, //bl
		0.5f, -0.7f, 0.0f, 0.0f, 1.0f, 0.0f, //br
		0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 1.0f, //tr
		-0.5f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f
	};
	vector<float> rectdoor = {
		-0.1f,-0.7f, 0.0f, 1.0f, 0.0f, 0.0f, //bl
		0.1f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f, //br
		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //tr
		-0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	vector<float> rectwindow = {
		-0.4f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //bl
		-0.2f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //br
		-0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //tr
		-0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	vector<float> rectwindow2 = {
		0.2f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //bl
		0.4f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //br
		0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //tr
		0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	vector<float> tri = {
		-0.7f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.7f, 0.3f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	DrawRectangle(rect);
	DrawRectangle(rectdoor);
	DrawRectangle(rectwindow);
	DrawRectangle(rectwindow2);
	DrawTriangle(tri);

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
	int colorLocation1 = glGetUniformLocation(shaders[0].ID, "color1");
	int colorLocation2 = glGetUniformLocation(shaders[0].ID, "color2");
	int colorLocation3 = glGetUniformLocation(shaders[0].ID, "color3");

	glUniform3f(colorLocation1, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f);
	glUniform3f(colorLocation2, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f);
	glUniform3f(colorLocation3, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f);
}

void Renderer::DrawTriangle(vector<float> verts)
{
	float toArray[18];
	copy(verts.begin(), verts.end(),toArray);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(toArray), &toArray, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

}

void Renderer::DrawRectangle(vector<float> verts)
{
	unsigned int indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	float toArray[24];
	copy(verts.begin(), verts.end(), toArray);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(toArray), &toArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

