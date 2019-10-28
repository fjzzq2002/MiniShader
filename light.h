#ifndef LIGHT_H
#define LIGHT_H

#include "vecmath.h"

struct PointLight
{
	Vector3f o, c;
	PointLight(const Vector3f& o = Vector3f::ZERO, const Vector3f& c = Vector3f::ZERO) :
		o(o), c(c) {}
};
#endif