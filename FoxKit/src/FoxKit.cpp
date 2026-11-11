
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "headers/Renderer.h"
using namespace std;

namespace
{
	static const Renderer* Render = new Renderer();

	static void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods) {
		if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			Render->SwitchProjection();
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			Render->SwitchWireframe();
		if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
			Render->SwitchFaceCulling();
	}
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



