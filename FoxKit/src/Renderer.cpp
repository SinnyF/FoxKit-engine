
#include "headers\Renderer.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

	static unsigned int VBO, EBO;
	static vector<Shader> shaders;
	static Camera camera;
	static vector<Mesh> objects;
	static double _time = 0;
	static PROJECTION proj = PERSPECTIVE;
	static bool faceCulling = true;

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
	
void Renderer::SwitchProjection() {
	switch (proj) {
	case PERSPECTIVE:
		proj = ORTOGRAPHIC;
		break;
	case ORTOGRAPHIC:
		proj = PERSPECTIVE;
		break;
	}
}

void Renderer::SwitchWireframe()
{
	GLint polygonMode;
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

	if (polygonMode == GL_LINE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (polygonMode == GL_FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::SwitchFaceCulling()
{
	if(faceCulling)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
	faceCulling = !faceCulling;
}

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
	AddObject(Mesh(PYRAMID));
	AddObject(Mesh(SPHERE));

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

	_time = glfwGetTime();

	//camera.position = vec3(8.0f, 6.0f, sin(_time) * 4.0f);
	

	
	
	mat4 model = mat4(1.0f);
	model = rotate(model, radians(90.0f * (float)_time), vec3(0,0, 1.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
	glUniformMatrix4fv(ProjectionlLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjection(proj)));
	Draw(objects[0]);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	Draw(objects[0]);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, vec3(sin(_time) + 1.0f, sin(_time) + 1.0f, sin(_time) + 1.0f)/2.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	Draw(objects[1]);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -2.0f, -2.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	Draw(objects[1]);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(sin(_time) * 5, cos(_time) * 5, sin(_time) * 5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	Draw(objects[2]);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(-2.0f, 3.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	Draw(objects[2]);
	
	glCheckError();
	glfwSwapBuffers(window);
}

void Renderer::Terminate() {
	for (int i = 0; i < objects.size(); i++) {
		glDeleteVertexArrays(1, &objects[i].VAO);
	}
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}

GLFWwindow* Renderer::GetWindow()
{
	return window;
}

void Renderer::AddObject(Mesh obj)
{
	glGenVertexArrays(1, &obj.VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glCheckError();
	glBindVertexArray(obj.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, obj.Vertices().size() * sizeof(float), &obj.Vertices()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.Indices().size() * sizeof(unsigned int), &obj.Indices()[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	objects.push_back(obj);
}
void Renderer::Draw(Mesh obj)
{
	glBindVertexArray(obj.VAO);
	glDrawElements(GL_TRIANGLES, obj.Indices().size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

