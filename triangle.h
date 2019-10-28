#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <cassert>
#include "vecmath.h"
#include <limits>
#include "object.h"
#include "ray.h"
#include <algorithm>

class Triangle : public Object {

public:
	Triangle() {}

	// a b c are three vertex positions of the triangle
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object(m) {
		v[0] = a;
		v[1] = b;
		v[2] = c;
		norm = Vector3f::cross(a - b,a - c);
		norm.normalize();
	}

	bool intersect(const Ray& ray, Hit& hit, float tmin) override {
		Matrix3f L(ray.d, v[2] - v[0], v[2] - v[1], 1);
		Vector3f R = v[2] - ray.o;
		bool sg; L = L.inverse(&sg);
		if (sg) return 0; R = L * R;
		if (R[1] + R[2] > 1 || R[1] < 0 || R[2] < 0) return 0;
		float t = R[0];
		if (t < tmin || t >= hit.t) return 0;
		hit.t = t; hit.obj = this; hit.norm = norm;
		return 1;
	}
	Vector3f norm;
	Vector3f v[3];
	BoundingBox getBound() override {
		return BoundingBox::v3(v[0], v[1], v[2]);
	}
};
#endif