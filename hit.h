#ifndef HIT_H
#define HIT_H
#include <cassert>
#include "utils.h"
#include <limits>
class Object;
class Hit {
public:
	Object*obj;
	float t;
	Vector3f norm;
	Vector2f pos;
	Hit(float t = std::numeric_limits<float>::max(),
		const Vector3f& norm=Vector3f::ZERO,
		const Vector2f& pos=Vector2f::ZERO,
		Object *obj=nullptr) :
		t(t), obj(obj), norm(norm), pos(pos) {}
};


#endif