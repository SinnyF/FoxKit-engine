#include "headers\Camera.h"

Camera::Camera()
{
	position = vec3(8.0f,6.0f,3.0f);
	target = vec3(0,0,0);
	up = vec3(0,0,1.0f);
}

mat4 Camera::GetView() const
{
	return glm::lookAt(position,target,up);
}

mat4 Camera::GetProjection(PROJECTION proj) const
{
	switch (proj) {
	case PERSPECTIVE:
		return perspective(radians(FOV), aspectRatio, nearPlane, farPlane);
		break;
	case ORTOGRAPHIC:
		float scale = 8.0f;
		return ortho(-1.366f * scale, 1.366f * scale, -0.768f * scale, 0.768f * scale, 0.0f, 1000.0f);
		break;
	}
}

void Camera::LookAt(const vec3& _target, const vec3& _up)
{
	target = _target;
	up = _up;
}
