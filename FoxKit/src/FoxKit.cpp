#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

	int main()
	{
		if (!glfwInit()) {
			std::cout << "Failed to initialize GLFW" << std::endl;
			return -1;
		}
		return 0;
	}



