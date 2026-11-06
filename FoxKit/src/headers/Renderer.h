#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include "headers\Shader.h"
#include "headers\Mesh.h"
#include "headers\Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static GLFWwindow* window;

class Renderer
{

public: 
	static int InitializeRenderer();
	static void Render();
	static void Terminate();
	static GLFWwindow* GetWindow();
	static void AddObject(Mesh obj);
	static void SwitchProjection();
	static void SwitchWireframe();
	static void SwitchFaceCulling();
private:
	
	static void Draw(Mesh obj);
};

