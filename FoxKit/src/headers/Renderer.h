#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Entity.h"

class Renderer
{

public: 
	static void WindowResized(GLFWwindow* window,int width, int height);
	static int InitializeRenderer(int width,int height);
	static void Render(const vector<Entity> &objects);
	static void Terminate(const vector<Entity> &objects);
	static GLFWwindow* GetWindow();
	static void AddObject(Entity obj, vector<Entity> &objects);
	static void SwitchProjection();
	static void SwitchWireframe();
	static void SwitchFaceCulling();
	static vector<Camera*> cameras;
	
private:
	static void Draw(Entity &obj);
	static vector<Shader> shaders;
	static double _time;
	static PROJECTION proj;
	static bool faceCulling;
	static GLFWwindow* window;
};

