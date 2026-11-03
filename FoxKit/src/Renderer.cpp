
#include "headers\Renderer.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

	static unsigned int VBO, VAO;
	static unsigned int EBO;
	static vector<Shader> shaders;
	static Camera camera;
	static vector<Mesh> objects;
	static double _time = 0;

	static float tempCols[9] = {0,0,0,0,0,0,0,0,0};

	GLenum glCheckError_(const char* file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
	
void WindowResized(GLFWwindow* window, int width, int height) 
{
	glViewport(0,0,width,height);
}

int Renderer::InitializeRenderer()
{
	cout << "Initializing Renderer..." << endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1366, 768, "FoxKit engine", NULL, NULL);
	

	if (window == NULL) {
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return 0;
	}

	camera = Camera();

	glViewport(0, 0, 1366, 768);
	glfwSetFramebufferSizeCallback(window, WindowResized);

	AddObject(Mesh(CUBE));

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glCheckError();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, objects[0].Vertices().size() * sizeof(float), &objects[0].Vertices()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objects[0].Indices().size() * sizeof(int), &objects[0].Indices()[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	glCheckError();

	shaders.push_back( Shader("src/shaders/base.vert", "src/shaders/base2.frag"));

	return 1;
}

void Renderer::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	shaders[0].use();

	int modelLocation = glGetUniformLocation(shaders[0].ID, "model");
	int viewLocation = glGetUniformLocation(shaders[0].ID, "view");
	int ProjectionlLocation = glGetUniformLocation(shaders[0].ID, "projection");

	_time += 0.01f;
	camera.position = vec3(glm::sin(_time), glm::sin(_time) + 3.0f, glm::sin(_time));

	mat4 model = mat4(1.0f);
	model = translate(model, vec3(0, 0, 0));
	model = rotate(model, radians(30.0f * (float)_time), vec3(0,0, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
	glUniformMatrix4fv(ProjectionlLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjection(PERSPECTIVE)));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, objects[0].Indices().size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glCheckError();
	glfwSwapBuffers(window);
}

void Renderer::Terminate() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}

GLFWwindow* Renderer::GetWindow()
{
	return window;
}

void Renderer::AddObject(Mesh obj)
{
	objects.push_back(obj);
}


