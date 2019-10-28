#ifndef OBJECT_H
#define OBJECT_H

#include <cassert>
#include "vecmath.h"
#include <limits>
#include "material.h"
#include "boundingbox.h"

class Ray;
class Hit;
class Object {
public:
	Material *m; //the material need to be manually assigned
	Object(Material*m=0):m(m) {}
	virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
	virtual BoundingBox getBound() = 0;
};

#endif