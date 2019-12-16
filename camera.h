#ifndef CAMERA_H
#define CAMERA_H
#include <cassert>
#include <string>
#include "utils.h"
#include "ray.h"

class Camera {
public:
	Camera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, int imgW, int imgH) {
		this->center = center;
		this->direction = direction.normalized();
		this->horizontal = Vector3f::cross(this->direction, up).normalized();
		this->up = Vector3f::cross(this->horizontal, this->direction);
		this->width = imgW;
		this->height = imgH;
	}

	virtual ~Camera() = default;
	virtual Ray generateRay(int x, int y, RNG_TYPE&rng) const = 0;
	virtual void dump(std::map<std::string, std::pair<ParamType, void*>>& t) = 0;

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	Vector3f center;
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;
	int width;
	int height;
};

enum CameraType { PERSPECTIVECAM = 0, THINLENSCAM = 1 };
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
	void dump(std::map<std::string, std::pair<ParamType, void*>>& t) override{
#define DUMP(p,q) t[p]=dump_element(q)
		DUMP("camera_type", (int)PERSPECTIVECAM);
		DUMP("camera_center", &center);
		DUMP("camera_direction", &direction);
		DUMP("camera_up", &up);
		DUMP("camera_horizontal", &horizontal);
		DUMP("camera_dist", &dist);
		DUMP("camera_width", &width);
		DUMP("camera_height", &height);
#undef DUMP
	}

	Ray generateRay(int x, int y, RNG_TYPE&rng) const override {
		Vector3f g(x+sqrtf(randf(rng))*(0.5-rng()%2) - (width - 1) / 2.0f,
			y+sqrtf(randf(rng))*(0.5-rng()%2) - (height - 1) / 2.0f, dist);
		g.normalize();
		return Ray(center, Matrix3f(horizontal, -up, direction, 1) * g);
	}
};

class ThinLensCamera : public Camera {

public:
	float dist;
	float length;
	float radius;

	ThinLensCamera(const Vector3f& center, const Vector3f& direction,
		const Vector3f& up, int imgW, int imgH, float d, float l, float r) :
		Camera(center, direction, up, imgW, imgH) {
		dist = d; length = l; radius = r;
	}


	void dump(std::map<std::string, std::pair<ParamType, void*>>& t) override {
#define DUMP(p,q) t[p]=dump_element(q)
		DUMP("camera_type", THINLENSCAM);
		DUMP("camera_center", &center);
		DUMP("camera_direction", &direction);
		DUMP("camera_up", &up);
		DUMP("camera_horizontal", &horizontal);
		DUMP("camera_dist", &dist);
		DUMP("camera_length", &length);
		DUMP("camera_radius", &radius);
		DUMP("camera_width", &width);
		DUMP("camera_height", &height);
#undef DUMP
	}

	Ray generateRay(int x, int y, RNG_TYPE& rng) const override {
		Vector3f g(x + sqrtf(randf(rng)) * (0.5 - rng() % 2) - (width - 1) / 2.0f,
			y + sqrtf(randf(rng)) * (0.5 - rng() % 2) - (height - 1) / 2.0f, dist);
		//Vector3f g((x+randf(rng)-0.5 - (width - 1) / 2.0f), (y+randf(rng)-0.5 - (height - 1) / 2.0f), dist);
		g.normalize();
		float a = randf(rng) * (2 * PI);
		Vector3f df = up * cosf(a) + horizontal * sinf(a); df *= radius*sqrtf(randf(rng));
		return Ray(center + (Matrix3f(horizontal, -up, direction, 1)*df), (Matrix3f(horizontal, -up, direction, 1) *
			(g * length - df)).normalized());
	}
};

#endif