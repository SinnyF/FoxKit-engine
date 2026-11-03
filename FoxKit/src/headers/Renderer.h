#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include "headers\Shader.h"
#include"headers\Mesh.h"

using namespace std;

static GLFWwindow* window;

class Renderer
{

public: 
	static int InitializeRenderer();
	static void Render();
	static void Terminate();
	static GLFWwindow* GetWindow();
	static void SetColors();
	static void AddObject(Mesh obj);
private:
	static vector<Mesh> objects;
	static void DrawTriangle(int shader, float offsetX, float offsetY, float offsetZ, float scale, float flipX = 1.0f, float flipY = 1.0f, float flipZ = 1.0f);
	static void DrawRectangle(int shader, float offsetX, float offsetY, float offsetZ, float scale);
    
};

