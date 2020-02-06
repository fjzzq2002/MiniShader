#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <cassert>
#include <limits>
#include <vector>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include "object.h"
#include "ray.h"
using namespace std;
#define TRIANGLE_REMIT

class Triangle : public Object {

public:
	Triangle() {}

	bool empty=false, use_n;
	bool has_uv = false;
	Vector2f uv[3];
	float area;

	void prepare_coords() {
		if (has_uv) return;
		Vector3f v1, v2;
		o = Vector3f::ZERO;
		Vector3f norm = Vector3f::cross(v[0] - v[1], v[0] - v[2]).normalized();
		float C = Vector3f::dot(v[0], norm);
		if (fabs(norm[0]) > fabs(norm[1]) && fabs(norm[0])>fabs(norm[2])) o[0] = C / norm[0];
		else if (fabs(norm[1]) > fabs(norm[0])&&fabs(norm[1])>fabs(norm[2])) o[1] = C / norm[1];
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
		area = Vector3f::cross(a - b, a - c).length() / 2;
		if (area < 1e-5)
		{
			empty = true;
			return;
		}
#ifdef TRIANGLE_REMIT
		if (m->is_brdf() && !((BRDFMaterial*)m)->emissionColor.is_empty())
			remit = 1;
#endif
		n[0] = Vector3f::cross(a - b,a - c).normalized(); use_n = false;
		if (m->need_coords()) prepare_coords();
	}
	// a b c are three vertex positions of the triangle
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c,
		const Vector3f& p, const Vector3f& q, const Vector3f& r, Material* m) :Object(m) {
		v[0] = a;
		v[1] = b;
		v[2] = c;
		area = Vector3f::cross(a - b, a - c).length() / 2;
		if (area == 0)
		{
			empty = true;
			return;
		}
		n[0] = p.normalized();
		n[1] = q.normalized();
		n[2] = r.normalized();
		use_n = true;
#ifdef TRIANGLE_REMIT
		if (m->is_brdf() && !((BRDFMaterial*)m)->emissionColor.is_empty())
			remit = 1;
#endif
		if (m->need_coords()) prepare_coords();
	}

	// a b c are three vertex positions of the triangle
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c,
		const Vector2f& u1, const Vector2f &u2, const Vector2f& u3,
		const Vector3f& p, const Vector3f& q, const Vector3f& r, Material* m) :Object(m) {
		v[0] = a;
		v[1] = b;
		v[2] = c;
		uv[0] = u1;
		uv[1] = u2;
		uv[2] = u3;
		has_uv = true;
		area = Vector3f::cross(a - b, a - c).length() / 2;
		if (area == 0)
		{
			empty = true;
			return;
		}
		n[0] = p.normalized();
		n[1] = q.normalized();
		n[2] = r.normalized();
		use_n = true;
#ifdef TRIANGLE_REMIT
		if (m->is_brdf() && !((BRDFMaterial*)m)->emissionColor.is_empty())
			remit = 1;
#endif
		if (m->need_coords()) prepare_coords();
	}

	bool intersect(const Ray& ray, Hit& hit, float tmin) override {
		if (empty) return 0;
		Matrix3f L(ray.d, v[2] - v[0], v[2] - v[1], 1);
		Vector3f R = v[2] - ray.o;
		bool sg; L = L.inverse(&sg);
		if (sg) return 0; R = L * R;
		if (R[1] + R[2] > 1 || R[1] < 0 || R[2] < 0) return 0;
		float t = R[0];
		if (t < tmin || t >= hit.t) return 0;
		hit.t = t; hit.obj = this;
		hit.norm = use_n?((R[1]*n[0]+R[2]*n[1]+(1-R[1]-R[2])*n[2]).normalized()):n[0];
		if (m->need_coords()) {
			if (!has_uv) {
				Vector3f p = matrix * (ray.o + ray.d * t - o);
				Vector3f tmp = ray.o + ray.d * t;
	//			cerr << tmp[0] << "," << tmp[1] << "," << tmp[2] << ":" << p[1] << "," << p[2] << "\n";
				hit.pos = Vector2f(p[1], p[2]);
			}
			else {
				hit.pos = uv[0] * R[1] + uv[1] * R[2] + uv[2] * (1 - R[1] - R[2]);
			}
		}
		else hit.pos = Vector2f(0, 0);
		return 1;
	}
	Vector3f n[3];
	Vector3f v[3];
	Vector3f o;
	Matrix3f matrix;
	BoundingBox getBound() override {
		if (empty) return BoundingBox::EMPTY;
		return BoundingBox::v3(v[0], v[1], v[2]);
	}
};
#endif