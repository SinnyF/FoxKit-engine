#include "headers\Camera.h"

Camera::Camera(const int _width,const int _height,const float _fov,const float _nearPlane,const float _farPlane)
{
	target = vec3(0, 0, 0);
	up = vec3(0, 1.0f, 0);
	aspectRatio = float(_width) / float(_height);
	FOV = _fov;
	nearPlane = _nearPlane;
	farPlane = _farPlane;
}

void Camera::Resize(const int _width,const int _height)
{
	aspectRatio = float(_width) / float(_height);
}

mat4 Camera::GetView() const
{
	return glm::lookAt(worldposition,target,up);
}

mat4 Camera::GetProjection(const PROJECTION proj) const
{
	float scale = 8.0f;
	switch (proj) {
	case ORTOGRAPHIC:
		return ortho(-1.366f * scale, 1.366f * scale, -0.768f * scale, 0.768f * scale, 0.0f, 1000.0f);
		break;
	case PERSPECTIVE:
		return perspective(radians(FOV), aspectRatio, nearPlane, farPlane);
		break;
	}
	return -1;
}

void Camera::LookAt(const vec3& _target, const vec3& _up)
{
	target = _target;
	up = _up;
}
