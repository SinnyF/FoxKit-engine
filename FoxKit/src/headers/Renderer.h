#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include "headers\Shader.h"

using namespace std;

static GLFWwindow* window;

struct mesh {
	vector<float> vertices;
};

class Renderer
{

public: 
	static int InitializeRenderer();
	static void Render();
	static void Terminate();
	static GLFWwindow* GetWindow();
	static void SetColors();
private:
	static vector<mesh> objects;
	static void DrawTriangle(vector<float> verts);
	static void DrawRectangle(vector<float> verts);
};

