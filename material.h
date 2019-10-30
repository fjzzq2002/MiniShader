#ifndef MATERIAL_H
#define MATERIAL_H
#include <algorithm>
#include "utils.h"
#include "ray.h"
#include "hit.h"
#include "light.h"
class Material {
public:
	Vector3f diffuseColor;
	Vector3f specularColor;
	float roughness;
	Vector3f emissionColor;
	Material(const Vector3f& d_color = Vector3f::ZERO, const Vector3f& s_color = Vector3f::ZERO,
		float roughness = 0, const Vector3f& e_color=Vector3f::ZERO) :
		diffuseColor(d_color), specularColor(s_color),
		roughness(roughness), emissionColor(e_color) {
	}

	Vector3f BRDF(const Vector3f& in, const Vector3f& out,const Hit& hit) {
		Vector3f L = out.normalized();
		Vector3f N = hit.norm;
		float LN = Vector3f::dot(L, N);
		if (LN <= 0) return Vector3f::ZERO;
		Vector3f R = 2 * LN * N - L, V = -in; V.normalize();
		Vector3f H = L + V; H.normalize();
		float HN = Vector3f::dot(H, N), VH = Vector3f::dot(V, H),
			VN = Vector3f::dot(V, N), LH=Vector3f::dot(L,H);
		if (VN < 0) return Vector3f::ZERO; //?
		float nhh = HN * HN;
		float D = expf((nhh - 1) / (roughness * roughness * nhh)) / (roughness * roughness * nhh * nhh);
//		float D = roughness * roughness / (PI * td * td);
		float u = 1 - VN; //u^5
		u *= u; u *= u; u *= 1 - VN;
		Vector3f F = specularColor + (1 - specularColor) * u;
		float G = std::min(std::min(HN * VN / VH, HN * LN / VH) * 2, float(1));
		return LN * (diffuseColor
			+ D * F * G / (4 * LN * VN));
	}
	/*
	Vector3f BRDFE(const Vector3f& in, const Vector3f& out, const Hit& hit) {
		Vector3f L = out.normalized();
		Vector3f N = hit.norm;
		float LN = Vector3f::dot(L, N);
		if (LN <= 0) return Vector3f::ZERO;
		Vector3f R = 2 * LN * N - L, V = -in; V.normalize();
		Vector3f H = L + V; H.normalize();
		float HN = Vector3f::dot(H, N), VH = Vector3f::dot(V, H), VN = Vector3f::dot(V, N);
		float nhh = HN * HN;
		float D = expf((nhh - 1) / (roughness * roughness * nhh)) / (roughness * roughness * nhh * nhh);
		//		float D = roughness * roughness / (PI * td * td);
		float u = 1 - LN; //u^5
		u *= u; u *= u; u *= 1 - LN;
		Vector3f F = specularColor + (1 - specularColor) * u;
		float G = std::min(std::min(HN * VN / VH, HN * LN / VH) * 2, float(1));
		return (diffuseColor
			+ D * F * G / (4 * LN * VN));
	}*/
	Vector3f Shade(const Ray& ray, const Hit& hit, const PointLight& l) {
		return BRDF(ray.d, l.o - ray.pos(hit.t), hit) * l.c;
	}

protected:
	float clamp(float x)
	{
		if (x < 0) return 0;
		if (x > 1) return 1;
		return x;
	}
};
#endif