
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "headers/Renderer.h"
using namespace std;

const Renderer* Render = new Renderer();;

void ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Render->SetColors();
}

int main()
	{		
		if (!Render->InitializeRenderer())
			return -1;
		glfwSetWindowShouldClose(Render->GetWindow(), false);

		while (!glfwWindowShouldClose(Render->GetWindow())) {
			ProcessInput(Render->GetWindow());
			Render->Render();
			glfwPollEvents();
			this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		Render->Terminate();
		return 0;
	}



