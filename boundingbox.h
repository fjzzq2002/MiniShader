#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "utils.h"
#include <cmath>
#include "ray.h"

class BoundingBox {
private:
	inline bool cmp(const Vector3f& a, const Vector3f& b)
	{
		return a[0] <= b[0] && a[1] <= b[1] && a[2] <= b[2];
	}
public:
	Vector3f a, b;
	//empty
	BoundingBox() { a[0] = a[1] = a[2] = 1e38; b[0] = b[1] = b[2] = -1e38; }
	//one point
	BoundingBox(Vector3f x) : a(x), b(x) {}
	//two corners
	BoundingBox(Vector3f a, Vector3f b) : a(a), b(b) {}
	//three points
	static BoundingBox v3(Vector3f v0, Vector3f v1, Vector3f v2)
	{
		Vector3f mi = v0, mx = v0;
		for (int c = 0; c < 3; ++c)
		{
			if (v1[c] < mi[c]) mi[c] = v1[c];
			if (v1[c] > mx[c]) mx[c] = v1[c];
		}
		for (int c = 0; c < 3; ++c)
		{
			if (v2[c] < mi[c]) mi[c] = v2[c];
			if (v2[c] > mx[c]) mx[c] = v2[c];
		}
		return BoundingBox(mi, mx);
	}
	static const BoundingBox FULL;
	static const BoundingBox EMPTY;
	BoundingBox operator | (BoundingBox bb)
	{
		for (int i = 0; i < 3; ++i)
			if (a[i] < bb.a[i]) bb.a[i] = a[i];
		for (int i = 0; i < 3; ++i)
			if (b[i] > bb.b[i]) bb.b[i] = b[i];
		return bb;
	}
	float size()
	{
		float s = 1;
		for (int i = 0; i < 3; ++i)
		{
			if (a[i] > b[i]) return 0;
			s *= b[i] - a[i] + float(1e-4);
		}
		return s;
	}
	bool intersect(const Ray& r, float tmin,float tmax) {
		Vector3f o = r.o, d = r.d;
		const float eps = 1e-2;
		if (a[0] - eps <= o[0] && o[0] <= b[0] + eps && a[1] - eps <= o[1] && o[1] <= b[1] + eps && a[2] - eps <= o[2] && o[2] <= b[2] + eps)
			return 1;
		if (a[0] > b[0]) return 0; //empty box
		float t;
		if (d[0] != 0.f)
		{
			if (d[0] > 0.f) t = (a[0] - o[0]) / d[0];
			else t = (b[0] - o[0]) / d[0];
			if (t >= tmin && t <= tmax)
			{
				Vector3f s = o + t * d;
				if (a[1] - eps <= s[1] && s[1] <= b[1] + eps && a[2] - eps <= s[2] && s[2] <= b[2] + eps)
					return 1;
			}
		}
		if (d[1] != 0.f)
		{
			if (d[1] > 0.f) t = (a[1] - o[1]) / d[1];
			else t = (b[1] - o[1]) / d[1];
			if (t >= tmin && t <= tmax)
			{
				Vector3f s = o + t * d;
				if (a[0] - eps <= s[0] && s[0] <= b[0] + eps && a[2] - eps <= s[2] && s[2] <= b[2] + eps)
					return 1;
			}
		}
		if (d[2] != 0.f)
		{
			if (d[2] > 0.f) t = (a[2] - o[2]) / d[2];
			else t = (b[2] - o[2]) / d[2];
			if (t >= tmin && t <= tmax)
			{
				Vector3f s = o + t * d;
				if (a[1] - eps <= s[1] && s[1] <= b[1] + eps && a[0] - eps <= s[0] && s[0] <= b[0] + eps)
					return 1;
			}
		}
		return 0;
	}
};
const BoundingBox BoundingBox::EMPTY(Vector3f(1e38, 1e38, 1e38), Vector3f(-1e38, -1e38, -1e38));
const BoundingBox BoundingBox::FULL(Vector3f(-1e38, -1e38, -1e38), Vector3f(1e38, 1e38, 1e38));


#endif