#ifndef RAY_H
#define RAY_H

#include <cassert>
#include <limits>
#include "utils.h"

class Ray {
public:
	Vector3f o, d;
	float tmax;
	Ray():tmax(std::numeric_limits<float>::max()) {}
	Ray(Vector3f o,Vector3f d,float tmax=std::numeric_limits<float>::max(),float t=0):
		o(o),d(d),tmax(tmax) {}
	Vector3f pos(float t) const { return o + d * t; }
};

#endif