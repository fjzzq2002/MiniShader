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

inline void createCoordinateSystem(const Vector3f& N, Vector3f& Nt, Vector3f& Nb)
{
	if (std::fabs(N.m_elements[0]) > std::fabs(N.m_elements[1]))
		Nt = Vector3f(N.m_elements[2], 0, -N.m_elements[0]) / sqrtf(N.m_elements[0] * N.m_elements[0] + N.m_elements[2] * N.m_elements[2]);
	else
		Nt = Vector3f(0, N.m_elements[2], -N.m_elements[1]) / sqrtf(N.m_elements[1] * N.m_elements[1] + N.m_elements[2] * N.m_elements[2]);
	Nb = Vector3f::cross(N, Nt);
}
template<class T>
std::vector<T> poly_mul(std::vector<T> a, std::vector<T> b) {
	if (a.empty()) return a;
	if (b.empty()) return b;
	std::vector<T> g(a.size() + b.size() - 1);
	for (int x = 0; x<int(a.size()); ++x)
		for (int y = 0; y<int(b.size()); ++y)
			g[x + y] = g[x + y] + a[x] * b[y];
	return g;
}
template<class T>
std::vector<T> poly_mul(std::vector<T> a, double b) {
	for (auto& x : a) x *= b;
	return a;
}
template<class T>
std::vector<T> poly_mul(std::vector<T> a, Vector2f b) {
	for (auto& x : a) x = x * b;
	return a;
}
template<class T>
std::vector<T> poly_add(std::vector<T> a, std::vector<T> b) {
	std::vector<T> g(std::max(a.size(), b.size()));
	for (int x = 0; x<int(a.size()); ++x) g[x] = g[x] + a[x];
	for (int x = 0; x<int(b.size()); ++x) g[x] = g[x] + b[x];
	return g;
}
template<class T>
std::vector<T> poly_minus(std::vector<T> a, std::vector<T> b) {
	std::vector<T> g(std::max(a.size(), b.size()));
	for (int x = 0; x<int(a.size()); ++x) g[x] = g[x] + a[x];
	for (int x = 0; x<int(b.size()); ++x) g[x] = g[x] - b[x];
	return g;
}
template<class T>
std::vector<T> poly_neg(std::vector<T> a) {
	for (auto& x : a) x = -x;
	return a;
}
double poly_calc(const std::vector<double>& v, double x) {
	double s = 0, u = 1;
	for (auto p : v)
		s += u * p, u *= x;
	return s;
}
#endif
