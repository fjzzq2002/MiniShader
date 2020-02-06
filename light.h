#ifndef LIGHT_H
#define LIGHT_H

#include "utils.h"

struct PointLight
{
	Vector3f o, c;
	PointLight(const Vector3f& o, const Vector3f& c = Vector3f::ZERO) :
		o(o), c(c) {}
};
#endif