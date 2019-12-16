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

	void prepare_coords() {
		Vector3f v1, v2;
		o = Vector3f::ZERO;
		if (fabs(norm[0]) > 1e-4) o[0] = d / norm[0];
		else if (fabs(norm[1]) > 1e-4) o[1] = d / norm[1];
		else o[2] = d / norm[2];
		createCoordinateSystem(norm, v1, v2);
		//norm,v1,v2 factorization
		matrix = Matrix3f(norm, v1, v2).inverse();
	}

	Plane(const Vector3f& norm, float d, Material*m):
		norm(norm),d(d),Object(m) {
		d /= this->norm.length();
		this->norm.normalize();
		if (m->need_coords()) prepare_coords();
	}

	bool intersect(const Ray& ray, Hit& h, float tmin) override {
		float g = d - Vector3f::dot(ray.o, norm), w = Vector3f::dot(ray.d, norm);
		if (w == 0) return 0;
		float t = g / w;
		if (t < tmin || t >= h.t) return 0;
		h.t = t; h.obj = this; h.norm = norm;if (m->need_coords()) {
			Vector3f p = matrix * (ray.o + ray.d * t - o);
//			std::cerr << p[0] << "," << p[1] << "," << p[2] << "\n";
			h.pos = Vector2f(p[1], p[2]);
		}
		else h.pos = Vector2f(0, 0);
		return 1;
	}
	BoundingBox getBound()
	{
		return BoundingBox::FULL;
	}

protected:
	Vector3f norm;
	float d;
	Vector3f o;
	Matrix3f matrix;

};
#endif