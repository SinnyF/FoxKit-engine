#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

static enum PROJECTION
{
	PERSPECTIVE,
	ORTOGRAPHIC
};

class Camera
{
public:
	Camera();

	mat4 GetView() const;
	mat4 GetProjection(PROJECTION proj) const;

	void LookAt(const vec3& target, const vec3& up);

	vec3 position;
	vec3 target;

	float FOV = 60.0f;
	float aspectRatio = 1366.0f/768.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
private:
	vec3 up;
};

