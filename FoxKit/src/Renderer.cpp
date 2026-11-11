
#include "headers\Renderer.h"
#include <iostream>
#include <algorithm>

static unsigned int VBO, EBO;
static vector<Shader> shaders;
static Camera camera;
static vector<Mesh> objects;
static double _time = 0;
static PROJECTION proj = PERSPECTIVE;
static bool faceCulling = true;

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

		AddObject(Mesh(SPHERE));
			AddObject(Mesh(CUBE));

		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glCheckError();

		shaders.push_back( Shader("src/shaders/base.vert", "src/shaders/base2.frag"));
		shaders[0].use();
		glCheckError();
		return 1;
}

void Renderer::Render()
{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		shaders[0].use();
		int modelLocation = glGetUniformLocation(shaders[0].ID, "model");
		int normalMLocation = glGetUniformLocation(shaders[0].ID, "normalM");
		int viewLocation = glGetUniformLocation(shaders[0].ID, "view");
		int projectionLocation = glGetUniformLocation(shaders[0].ID, "projection");
		int viewPosLocation = glGetUniformLocation(shaders[0].ID, "viewPos");
		int ambientLightLocation = glGetUniformLocation(shaders[0].ID, "ambientLight");
		int lightColorLocation = glGetUniformLocation(shaders[0].ID, "lightColor");
		int colorLocation = glGetUniformLocation(shaders[0].ID, "color");
		
		glUniform3f(lightColorLocation, 1.0f,1.0f,1.0f);
		glUniform3f(ambientLightLocation, 0.2f,0.2f,0.2f);
		glUniform3f(colorLocation, 0.0f,1.0f,1.0f);
		_time = glfwGetTime();
		
		int lightLocation = glGetUniformLocation(shaders[0].ID, "lightPos");
		glUniform3f(lightLocation, sin(_time)*6, 2.0f, cos(_time)*6);

		mat4 model = mat4(1.0f);
		model = rotate(model, radians(15.0f * (float)_time), vec3(0,1.0f, 0));
		
		camera.position = vec3(4, 2 ,4);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjection(proj)));
		glUniform3f(viewPosLocation, camera.position.x, camera.position.y, camera.position.z);
		mat3 normalMatrix = transpose(inverse(model));
		glUniformMatrix3fv(normalMLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		Draw(objects[1]);
		glCheckError();
		glfwSwapBuffers(window);
}

void Renderer::Terminate() 
{
		for (Mesh& object : objects)
		{
			glDeleteVertexArrays(1, &object.VAO);
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
		//Position attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//Normal attributes
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		objects.push_back(obj);
}

void Renderer::Draw(Mesh &obj)
{
		glBindVertexArray(obj.VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(obj.Indices().size()), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
}

