#include <vector>
#include <chrono>
#include <thread>
#include <set>
#include "headers/Renderer.h"
#include "headers/Entity.h"
#include "headers/Camera.h"
using namespace std;

double lastX = 0.0, lastY = 0.0;
float cameraYaw,cameraPitch, cameraYaw2, cameraPitch2;
vec3 cameraFront, cameraFront2;
vec2 cameraDirection2;
vec3 movementInput = vec3(0.0f), movementInput2 = vec3(0.0f);
bool Player2Loaded = true;

void SwitchPlayer2();

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
{
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		Renderer::SwitchProjection();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
		Renderer::SwitchWireframe();
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
		Renderer::SwitchFaceCulling();
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		movementInput.x = 1.0;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		movementInput.x = -1.0;
	if ((key == GLFW_KEY_W || key == GLFW_KEY_S) && action == GLFW_RELEASE)
		movementInput.x = 0;
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		movementInput.z = -1.0;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		movementInput.z = 1.0;
	if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_RELEASE)
		movementInput.z = 0;
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		SwitchPlayer2();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cameraYaw   += xoffset;
	cameraPitch += yoffset;

	cameraPitch = clamp(cameraPitch, -89.0f, 89.0f);

	vec3 direction;
	direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
	direction.y = sin(radians(cameraPitch));
	direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
	cameraFront = normalize(direction);
}

void gamepadInput(const int id)
{
	GLFWgamepadstate state;
	if (glfwGetGamepadState(id,&state))
	{
		movementInput2.x = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
		movementInput2.z = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
		cameraDirection2.x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		cameraDirection2.y = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
		if (glm::length(cameraDirection2) < 0.1f)
			cameraDirection2 = vec2(0);
		cameraDirection2 *= 2.0;
		cameraYaw2 += cameraDirection2.x;
		cameraPitch2 += cameraDirection2.y;
		vec3 direction;
		direction.x = cos(radians(cameraYaw2)) * cos(radians(cameraPitch2));
		direction.y = sin(radians(cameraPitch2));
		direction.z = sin(radians(cameraYaw2)) * cos(radians(cameraPitch2));
		cameraFront2 = normalize(direction);
	}
}
vector<Entity*> objects;
Entity* player2;
Entity* head2;
int main()
{	
	if (!Renderer::InitializeRenderer(1366,768))
		return -1;
	glfwSetFramebufferSizeCallback(Renderer::GetWindow(), Renderer::WindowResized);
	glfwSetWindowShouldClose(Renderer::GetWindow(), false);
	glfwSetKeyCallback(Renderer::GetWindow(), key_callback);
	glfwSetInputMode(Renderer::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	glfwSetCursorPosCallback(Renderer::GetWindow(), mouse_callback); 
	
	Renderer::AddObject(new Entity("bust.obj", 0, "Textures/bust_diff.jpg", "Textures/bust_nor2.png"), objects);
	Renderer::AddObject(new Entity("sharpcube.obj", 0, "Textures/rust_diff.png","Textures/rust_nor.png"), objects);
	Renderer::AddObject(new Entity("quad.obj", 0, "Textures/pavers_diff.png", "Textures/pavers_nor.png"), objects);
	auto it = next(objects.begin(), 0);
	objects[0]->uniformsf.emplace_back("specularPower",256);
	objects[0]->uniformsf.emplace_back("specularStrength",1);
	objects[0]->uniformsf.emplace_back("tiling",1);
	objects[1]->uniformsf.emplace_back("specularPower",32);
	objects[1]->uniformsf.emplace_back("specularStrength",0.7);
	objects[1]->uniformsf.emplace_back("tiling",1);
	objects[2]->uniformsf.emplace_back("specularPower",64);
	objects[2]->uniformsf.emplace_back("specularStrength",0.8);
	objects[2]->uniformsf.emplace_back("tiling",50);
	objects[1]->setParent(objects[0]);
	Camera *camera = new Camera(1366,768);
	Camera *camera2 = new Camera(1366,768);
	Renderer::cameras.push_back(camera);
	Renderer::cameras.push_back(camera2);
	
	Entity* player = new Entity("cylinder.obj");
	player->uniformsv3.emplace_back("color",vec3(0,0,1));
	Entity* head = new Entity("camera.obj");
	head->uniformsv3.emplace_back("color",vec3(0.2,0.2,0.2));
	head->setParent(player);
	head->Move(vec3(0,1,0));
	player2 = new Entity("cylinder.obj");
	player2->uniformsv3.emplace_back("color",vec3(1,0,0));
	head2 = new Entity("camera.obj");
	head2->uniformsv3.emplace_back("color",vec3(0.2,0.2,0.2));
	head2->setParent(player2);
	head2->Move(vec3(0,1,0));
	objects[2]->Scale(vec3(50));
	
	Renderer::AddObject(player, objects);
	Renderer::AddObject(player2, objects);
	Renderer::AddObject(head, objects);
	Renderer::AddObject(head2, objects);
	
	camera->SetPosition(vec3(0,1,0));
	camera->setParent(player);
	camera2->SetPosition(vec3(0,1,0));
	camera2->setParent(player2);
	player->Move(vec3(1,0,0));
	player2->Move(vec3(-1,0,0));
	
	Renderer::NewPlayer(true);
	
	double _time = glfwGetTime();
	float degrees=0;
	while (!glfwWindowShouldClose(Renderer::GetWindow())) {
		glfwPollEvents();
		
		if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1) && Player2Loaded)
		{
			gamepadInput(GLFW_JOYSTICK_1);
		}
		vec3 facing = normalize(vec3(cameraFront.x,0.0,cameraFront.z));
		vec3 rightfacing = normalize(cross(cameraFront, vec3(0,1,0)));
		
		vec3 facing2 = normalize(vec3(cameraFront2.x,0.0,cameraFront2.z));
		vec3 rightfacing2 = normalize(cross(cameraFront2, vec3(0,1,0)));
		
		double deltaTime = glfwGetTime()-_time;
		_time = glfwGetTime();
		degrees += deltaTime * 6.0;
		player->SetRotation(vec3(0,-cameraYaw,0));
		head->SetRotation(vec3(0,0,cameraPitch));
		player2->SetRotation(vec3(0,-cameraYaw2,0));
		head2->SetRotation(vec3(0,0,cameraPitch2));
		
		objects[0]->SetPosition(vec3(0,sin(_time)*0.2 + 1,0));
		objects[0]->Rotate(vec3(45*deltaTime,0,0));
		objects[1]->SetPosition(vec3(sin(radians(degrees * 20)) * 0.5,0.2,cos(radians(degrees * 20)) * 0.5));
		objects[1]->Rotate(vec3(0,205*deltaTime,0));
		
		player->Move(vec3(movementInput.x * facing + movementInput.z*rightfacing) *(float)deltaTime * 5.0f);
		if (glm::length(movementInput2) < 0.1f)
			movementInput2 = vec3(0,0,0);
		player2->Move(vec3(movementInput2.x * facing2 + movementInput2.z*rightfacing2) *(float)deltaTime * 5.0f);
		camera->LookAt(camera->getWorldPosition()+cameraFront, vec3(0,1,0));
		camera2->LookAt(camera2->getWorldPosition()+cameraFront2, vec3(0,1,0));
		Renderer::Render(objects);
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	Renderer::Terminate(objects);
	return 0;
}

void SwitchPlayer2()
{
	if (Player2Loaded)
	{
		objects.erase(find(objects.begin(), objects.end(), player2));
		objects.erase(find(objects.begin(), objects.end(), head2));
	}
	else
	{
		objects.push_back(player2);
		objects.push_back(head2);
	}
	Player2Loaded = !Player2Loaded;
	Renderer::NewPlayer(Player2Loaded);
}

