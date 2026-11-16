#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"
#include "headers\Renderer.h"
#include <iostream>
#include <algorithm>

static vector<Shader> shaders;
static Camera camera;
static vector<Entity> objects;
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

	AddObject(Entity("sharpcube.obj", "Textures/pavers_diff.png"));
	objects[0].uniformsf.push_back(make_tuple("specularPower", 32));
	objects[0].uniformsf.push_back(make_tuple("specularStrength", 0.4));
	AddObject(Entity("sharpcube.obj", "Textures/rust_diff.png"));
	AddObject(Entity("bust.obj","Textures/bust_diff.jpg"));
	objects[2].uniformsf.push_back(make_tuple("specularPower", 256));
	objects[2].uniformsf.push_back(make_tuple("specularStrength", 0.9));
	AddObject(Entity("cat.obj","Textures/cat_diff.jpg"));
	objects[3].uniformsf.push_back(make_tuple("specularPower", 16));
	objects[3].uniformsf.push_back(make_tuple("specularStrength", 0.6));
	AddObject(Entity("tinker.obj", "", 1));
	AddObject(Entity("cube.obj", ""));
	AddObject(Entity("smoothcube.obj", ""));
	
	objects[0].Move(vec3(1,0,5));
	objects[0].Scale(vec3(0.5));
	objects[0].Rotate(vec3(30,-60,0));
	objects[1].Move(vec3(1.5,0,4));
	objects[1].Rotate(vec3(-30,120,0));
	objects[1].Scale(vec3(0.5));
	objects[2].Move(vec3(0,0,0));
	objects[2].Scale(vec3(10));
	objects[2].Rotate(vec3(0,75,0));
	objects[3].Move(vec3(2,0,1));
	objects[3].Scale(vec3(10));
	objects[4].Move(vec3(100,-20,0));
	objects[4].Scale(vec3(1));
	objects[4].Rotate(vec3(-90,0,-90));

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glCheckError();

	shaders.push_back( Shader("src/shaders/base.vert", "src/shaders/base2.frag"));
	shaders.push_back( Shader("src/shaders/base.vert", "src/shaders/base.frag"));
	shaders[0].use();
	
	int ambientLightLocation = glGetUniformLocation(shaders[0].ID, "ambientLight");
	int lightColorLocation = glGetUniformLocation(shaders[0].ID, "lightColor");
	int colorLocation = glGetUniformLocation(shaders[0].ID, "color");
	glCheckError();
	glUniform3f(lightColorLocation, 1.0f,1.0f,1.0f);
	glUniform3f(ambientLightLocation, 0.2f,0.2f,0.2f);
	glUniform3f(colorLocation, 0.0f,1.0f,1.0f);
	glCheckError();
	return 1;
}

void Renderer::Render()
{
	shaders[0].use();
	glClearColor(0.f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glCheckError();
	
	int viewLocation = glGetUniformLocation(shaders[0].ID, "view");
	int projectionLocation = glGetUniformLocation(shaders[0].ID, "projection");
	int viewPosLocation = glGetUniformLocation(shaders[0].ID, "viewPos");
	int lightLocation = glGetUniformLocation(shaders[0].ID, "lightPos");
	glUniform3f(lightLocation, 6, 2.0f, 6);
	glCheckError();
	camera.position = vec3(sin(_time * 0.5) * 8, 2 ,cos(_time * 0.5) * 8);
	camera.LookAt(vec3(0,3,0), vec3(0,1,0));
	glCheckError();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjection(proj)));
	glUniform3f(viewPosLocation, camera.position.x, camera.position.y, camera.position.z);
	glCheckError();
	
	shaders[1].use();
	viewLocation = glGetUniformLocation(shaders[1].ID, "view");
	projectionLocation = glGetUniformLocation(shaders[1].ID, "projection");
	viewPosLocation = glGetUniformLocation(shaders[1].ID, "viewPos");
	glCheckError();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjection(proj)));
	glUniform3f(viewPosLocation, camera.position.x, camera.position.y, camera.position.z);
	glCheckError();
	_time = glfwGetTime();
	int timeLocation = glGetUniformLocation(shaders[1].ID, "time");
	glUniform1f(timeLocation, _time);
	glCheckError();
	
	for (auto obj : objects)
	{
		Draw(obj);
	}
	
	glCheckError();
	glfwSwapBuffers(window);
}

void Renderer::Terminate() 
{
	for (auto obj : objects)
	{
		glDeleteTextures(1, &obj.textureID);
	}
	for (auto obj: shaders)
	{
		glDeleteShader(obj.ID);
	}
	Mesh::CleanUP();
	glfwTerminate();
}

GLFWwindow* Renderer::GetWindow()
{
	return window;
}

void Renderer::AddObject(Entity obj)
{
	Mesh::LoadMesh(obj.modelPath);
	if (!obj.texturePath.empty())
		obj.textureID = Mesh::LoadTexture(obj.texturePath);
	glCheckError();
	objects.push_back(obj);
}

void Renderer::Draw(Entity &obj)
{
	shaders[obj.shaderID].use();
	for (auto tuple : obj.uniformsf)
	{
		int location = glGetUniformLocation(shaders[obj.shaderID].ID, get<0>(tuple).c_str());
		glUniform1f(location, get<1>(tuple));
	}
	int modelLocation = glGetUniformLocation(shaders[obj.shaderID].ID, "model");
	int normalMLocation = glGetUniformLocation(shaders[obj.shaderID].ID, "normalM");
	glCheckError();
	mat3 normalM = transpose(inverse(obj.modelMatrix));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(obj.modelMatrix));
	glUniformMatrix3fv(normalMLocation, 1, GL_FALSE, glm::value_ptr(normalM));
	glCheckError();
	Mesh* mesh = Mesh::LoadMesh(obj.modelPath);
	if (!obj.texturePath.empty())
		glBindTexture(GL_TEXTURE_2D, obj.textureID);
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->Indices().size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glCheckError();
}

