
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "headers/Renderer.h"
using namespace std;

const Renderer* Render = new Renderer();;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		Render->SwitchProjection();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
	{		
		if (!Render->InitializeRenderer())
			return -1;
		glfwSetWindowShouldClose(Render->GetWindow(), false);
		glfwSetKeyCallback(Render->GetWindow(), key_callback);
		while (!glfwWindowShouldClose(Render->GetWindow())) {
			Render->Render();
			glfwPollEvents();
			this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		Render->Terminate();
		return 0;
	}



