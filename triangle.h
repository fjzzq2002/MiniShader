#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <cassert>
#include "utils.h"
#include <limits>
#include "object.h"
#include "ray.h"
#include <algorithm>
#include <iostream>
using namespace std;

class Triangle : public Object {

public:
	Triangle() {}

	void prepare_coords() {
		Vector3f v1, v2;
		o = Vector3f::ZERO;
		float C = Vector3f::dot(v[0], norm);
		if (fabs(norm[0]) > 1e-4) o[0] = C / norm[0];
		else if (fabs(norm[1]) > 1e-4) o[1] = C / norm[1];
		else o[2] = C / norm[2];
		createCoordinateSystem(norm, v1, v2);
		//norm,v1,v2 factorization
		matrix = Matrix3f(norm, v1, v2).inverse();
		/*
		cerr << norm[0] << "," << norm[1] << "," << norm[2] << " ";
		cerr << v1[0] << "," << v1[1] << "," << v1[2] << " ";
		cerr << v2[0] << "," << v2[1] << "," << v2[2] << " ";
		cerr << o[0] << " " << o[1] << " " << o[2] << "\n";*/
	}

	// a b c are three vertex positions of the triangle
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object(m) {
		v[0] = a;
		v[1] = b;
		v[2] = c;
		norm = Vector3f::cross(a - b,a - c);
		norm.normalize();
		if (m->need_coords()) prepare_coords();
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
		if (m->need_coords()) {
			Vector3f p = matrix * (ray.o + ray.d * t - o);
			Vector3f tmp = ray.o + ray.d * t;
//			cerr << tmp[0] << "," << tmp[1] << "," << tmp[2] << ":" << p[1] << "," << p[2] << "\n";
			hit.pos = Vector2f(p[1], p[2]);
		}
		else hit.pos = Vector2f(0, 0);
		return 1;
	}
	Vector3f norm;
	Vector3f v[3];
	Vector3f o;
	Matrix3f matrix;
	BoundingBox getBound() override {
		return BoundingBox::v3(v[0], v[1], v[2]);
	}
};
#endif