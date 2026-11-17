#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"
#include "headers/Renderer.h"
#include <iostream>
#include <algorithm>
#include <complex>

vector<Shader> Renderer::shaders;
vector<Camera*> Renderer::cameras;

GLFWwindow* Renderer::window;
PROJECTION Renderer::proj = PERSPECTIVE;

double Renderer::_time = 0;
bool Renderer::faceCulling = true;

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

void Renderer::WindowResized(GLFWwindow* window,const int width,const int height) 
{
	glViewport(0,0,width,height);
	cameras[0]->Resize(width,height);
}

int Renderer::InitializeRenderer(const int width,const int height)
{
	cout << "Initializing Renderer..." << endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "FoxKit engine", NULL, NULL);
		
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
	glViewport(0, 0, width, height);

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

void Renderer::Render(const vector<Entity> &objects)
{
	glClearColor(0.f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glCheckError();
	shaders[0].use();	
	int viewLocation = glGetUniformLocation(shaders[0].ID, "view");
	int projectionLocation = glGetUniformLocation(shaders[0].ID, "projection");
	int viewPosLocation = glGetUniformLocation(shaders[0].ID, "viewPos");
	int lightLocation = glGetUniformLocation(shaders[0].ID, "lightDir");
	vec3 lightDir = normalize(vec3(10,1,10));
	glUniform3f(lightLocation,lightDir.x,lightDir.y,lightDir.z);
	glCheckError();
	
	glCheckError();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(cameras[0]->GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(cameras[0]->GetProjection(proj)));
	glUniform3f(viewPosLocation, cameras[0]->getPosition().x, cameras[0]->getPosition().y, cameras[0]->getPosition().z);
	glCheckError();
	
	shaders[1].use();
	viewLocation = glGetUniformLocation(shaders[1].ID, "view");
	projectionLocation = glGetUniformLocation(shaders[1].ID, "projection");
	viewPosLocation = glGetUniformLocation(shaders[1].ID, "viewPos");
	glCheckError();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(cameras[0]->GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(cameras[0]->GetProjection(proj)));
	glUniform3f(viewPosLocation, cameras[0]->getPosition().x, cameras[0]->getPosition().y, cameras[0]->getPosition().z);
	glCheckError();
	_time = glfwGetTime();
	int timeLocation = glGetUniformLocation(shaders[1].ID, "time");
	glUniform1f(timeLocation, (float)_time);
	glCheckError();
	
	for (auto obj : objects)
	{
		Draw(obj);
	}
	
	glCheckError();
	glfwSwapBuffers(window);
}

void Renderer::Terminate(const vector<Entity> &objects) 
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

void Renderer::AddObject(Entity obj, vector<Entity> &objects)
{
	Mesh::LoadMesh(obj.modelPath);
	if (!obj.texturePath.empty())
		obj.textureID = Mesh::LoadTexture(obj.texturePath);
	if (!obj.normalPath.empty())
		obj.normalID = Mesh::LoadTexture(obj.normalPath);
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
	int textureBoolLocation = glGetUniformLocation(shaders[obj.shaderID].ID, "textureOn");
	int normalBoolLocation = glGetUniformLocation(shaders[obj.shaderID].ID, "normalOn");
	glCheckError();
	mat3 normalM = transpose(inverse(mat3(obj.modelMatrix)));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(obj.modelMatrix));
	glUniformMatrix3fv(normalMLocation, 1, GL_FALSE, glm::value_ptr(normalM));
	glCheckError();
	Mesh* mesh = Mesh::LoadMesh(obj.modelPath);
	if (!obj.texturePath.empty())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj.textureID);
		int textureLocation = glGetUniformLocation(shaders[obj.shaderID].ID, "diffuseMap");
		glUniform1i(textureLocation, 0);
		glUniform1i(textureBoolLocation, true);
	}
	else
		glUniform1i(textureBoolLocation, false);
	
	if (!obj.normalPath.empty())
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, obj.normalID);
		int textureLocation = glGetUniformLocation(shaders[obj.shaderID].ID, "normalMap");
		glUniform1i(textureLocation, 1);
		glUniform1i(normalBoolLocation, true);
	}
	else
		glUniform1i(normalBoolLocation, false);
	
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indicies), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glCheckError();
}

