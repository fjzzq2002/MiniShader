#ifndef PLANE_H
#define PLANE_H
#include <cassert>
#include "utils.h"
#include <limits>
#include "object.h"
#include "ray.h"
class Plane : public Object {
public:
	Plane() {}

	Plane(const Vector3f& norm, float d, Material*m):
		norm(norm),d(d),Object(m) {}

	bool intersect(const Ray& ray, Hit& h, float tmin) override {
		float g = d - Vector3f::dot(ray.o, norm), w = Vector3f::dot(ray.d, norm);
		if (w == 0) return 0;
		float t = g / w;
		if (t < tmin || t >= h.t) return 0;
		h.t = t; h.obj = this; h.norm = norm;
		return 1;
	}
	BoundingBox getBound()
	{
		return BoundingBox::FULL;
	}

protected:
	Vector3f norm;
	float d;

};
#endif