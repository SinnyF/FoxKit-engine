
#include <iostream>
#include "headers/Renderer.h"

void ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
	{
		Renderer* rend = new Renderer();
		
		if (!rend->InitializeRenderer())
			return -1;
		glfwSetWindowShouldClose(rend->GetWindow(), false);

		float vertices[] = {
			-0.5f,-0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};


		while (!glfwWindowShouldClose(rend->GetWindow())) {
			ProcessInput(rend->GetWindow());
			rend->Render();
			glfwPollEvents();
		}

		rend->Terminate();
		return 0;
	}



