#ifndef UTILS_H
#define UTILS_H

#define PI 3.141592653589793238462643383f
#include <random>
#include <map>
#include <string>
#include <algorithm>
#include "Matrix2f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"
#include "Quat4f.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
enum ParamType { PT_INT=0, PT_FLOAT=1, PT_VECTOR3F=2 };
typedef std::map<std::string, std::pair<ParamType, void*>> PARAMLIST_TYPE;
std::pair<ParamType, void*> dump_element(int s)
{
	return std::make_pair(PT_INT, (void*)(new int(s)));
}
std::pair<ParamType, void*> dump_element(int* s)
{
	return std::make_pair(PT_INT, (void*)s);
}
std::pair<ParamType, void*> dump_element(float* s)
{
	return std::make_pair(PT_FLOAT, (float*)s);
}
std::pair<ParamType, void*> dump_element(Vector3f* s)
{
	return std::make_pair(PT_VECTOR3F, (Vector3f*)s);
}
#define RNG_TYPE std::mt19937
inline float randf(RNG_TYPE& g)
{
	return double(g() - g.min()) / (g.max() - g.min());
}
#endif
