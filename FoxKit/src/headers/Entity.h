#pragma once
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;

class Entity
{
public:
	void Translate(vec3 move, vec3 rotate = vec3(0));
	void Rotate(vec3 rotate);
	void Scale(vec3 scale);
protected:
	vec3 position = vec3(0);
	Mesh* mesh = nullptr;
};

