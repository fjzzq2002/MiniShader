#ifndef HIT_H
#define HIT_H
#include <cassert>
#include "vecmath.h"
#include <limits>
class Object;
class Hit {
public:
	Object*obj;
	float t;
	Vector3f norm;
	Hit(float t = std::numeric_limits<float>::max(),
		const Vector3f& norm=Vector3f::ZERO, Object *obj=nullptr) :
		t(t), obj(obj), norm(norm) {}
};


#endif