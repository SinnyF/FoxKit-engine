#pragma once

#include <glad/glad.h>
#include <glfw3.h>

static GLFWwindow* window;

class Renderer
{
public: 
	static int InitializeRenderer();
	static void Render();
	static void Terminate();
	static GLFWwindow* GetWindow();
};

