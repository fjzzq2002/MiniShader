#ifndef SPHERE_H
#define SPHERE_H
#include <cassert>
#include "utils.h"
#include <limits>
#include "object.h"
#include "ray.h"
#include <algorithm>
#include <iostream>

class Sphere : public Object {
public:
	float r;
	Vector3f o;
	Sphere() {
		r = 1;
		o = Vector3f(0, 0, 0);
	}

	Sphere(const Vector3f& o,float r, Material* m) : o(o), r(r),Object(m) {}

	bool intersect(const Ray& ray, Hit& h, float tmin) override {
		Vector3f p = ray.o - o;
		float C = p.squaredLength() - r * r;
		float B = 2 * Vector3f::dot(p, ray.d);
		float A = ray.d.squaredLength();
		float dt = B * B - 4 * A * C;
		if (dt < 0) return 0;
		dt = sqrt(dt);
		float s[2] = { (-B - dt) / (2 * A),(-B + dt) / (2 * A) };
		if (s[0] > s[1]) std::swap(s[0], s[1]);
		for (float t : s)
		{
			if (t < tmin || t >= h.t) continue;
			Vector3f norm = ray.o + ray.d * t - o;
			norm.normalize();
			h.t = t; h.obj = this; h.norm = norm;
			return 1;
		}
		return 0;
	}
	BoundingBox getBound()
	{
        return BoundingBox(Vector3f(o[0]-r,o[1]-r,o[2]-r),
        Vector3f(o[0]+r,o[1]+r,o[2]+r));
	}
};
#endif