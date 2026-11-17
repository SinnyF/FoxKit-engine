
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "headers/Renderer.h"
#include "headers/Entity.h"
#include "headers/Camera.h"
using namespace std;

namespace
{
	static void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods) {
		if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			Renderer::SwitchProjection();
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			Renderer::SwitchWireframe();
		if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
			Renderer::SwitchFaceCulling();
	}
}

int main()
{
	vector<Entity> objects;
	
	if (!Renderer::InitializeRenderer(1366,768))
		return -1;
	glfwSetFramebufferSizeCallback(Renderer::GetWindow(), Renderer::WindowResized);
	glfwSetWindowShouldClose(Renderer::GetWindow(), false);
	glfwSetKeyCallback(Renderer::GetWindow(), key_callback);
	
	//Renderer::AddObject(Entity("cube.obj", 0, "Textures/bust_diff.jpg"), objects);
	Renderer::AddObject(Entity("bust.obj", 0, "Textures/bust_diff.jpg"), objects);
	Renderer::AddObject(Entity("sharpcube.obj", 0, "Textures/rust_diff.png"), objects);
	objects[1].setParent(&objects[0]);
	Camera *camera = new Camera(1366,768);
	Renderer::cameras.push_back(camera);
	camera->LookAt(vec3(0,0.2,0), vec3(0,1,0));
	
	double _time = glfwGetTime();
	double deltaTime=0;
	float degrees=0;
	while (!glfwWindowShouldClose(Renderer::GetWindow())) {
		glfwPollEvents();
		deltaTime = glfwGetTime()-_time;
		_time = glfwGetTime();
		degrees += deltaTime * 6.0;
		camera->Move(vec3(sin(radians(degrees)) * 2, 0.5 ,cos(radians(degrees)) * 2));
		objects[0].Move(vec3(0,sin(_time),0));
		objects[0].Rotate(vec3(degrees*10,0,0));
		objects[1].Move(vec3(sin(radians(degrees * 20)) * 0.5,0.2,cos(radians(degrees * 20)) * 0.5));
		objects[1].Rotate(vec3(0,degrees*100,degrees*50));
		Renderer::Render(objects);
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	Renderer::Terminate(objects);
	return 0;
}



