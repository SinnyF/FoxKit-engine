#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"

enum PROJECTION
{
	PERSPECTIVE,
	ORTOGRAPHIC
};

using namespace glm;

class Camera : public Entity
{
public:
	Camera(int _width, int _height, float _fov = 60.0f, float _nearPlane = 0.1f, float _farPlane = 500.0f);
	void Resize(int _width, int _height);
	mat4 GetView() const;
	mat4 GetProjection(PROJECTION proj) const;
	void LookAt(const vec3& _target, const vec3& _up);
	
	vec3 target;
	
private:
	vec3 up;
	float FOV, aspectRatio, nearPlane, farPlane;
};

