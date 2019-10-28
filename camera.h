#ifndef CAMERA_H
#define CAMERA_H
#include <cassert>
#include "vecmath.h"
#include "ray.h"

class Camera {
public:
	Camera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, int imgW, int imgH) {
		this->center = center;
		this->direction = direction.normalized();
		this->horizontal = Vector3f::cross(this->direction, up);
		this->up = Vector3f::cross(this->horizontal, this->direction);
		this->width = imgW;
		this->height = imgH;
	}

	virtual Ray generateRay(const Vector2f& point) const = 0;
	virtual ~Camera() = default;

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	Vector3f center;
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;
	int width;
	int height;
};

class PerspectiveCamera : public Camera {

public:
	float dist;

	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,
		const Vector3f& up, int imgW, int imgH, float f, bool is_angle) :
		Camera(center, direction, up, imgW, imgH) {
		if (is_angle)
		{
			float c = imgH / tanf(f / 2), p = imgW;
			dist = sqrtf(c * c - p * p) / 2;
		}
		else dist = f;
	}

	Ray generateRay(const Vector2f& point) const override {
		Vector3f g((point[0] - (width - 1) / 2.0f), (point[1] - (height - 1) / 2.0f), dist);
		g.normalize();
		return Ray(center, Matrix3f(horizontal, -up, direction, 1) * g); //?
	}
};
#endif