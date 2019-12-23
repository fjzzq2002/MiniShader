#ifndef MATERIAL_H
#define MATERIAL_H
#include <algorithm>
#include "utils.h"
#include "runtime.h"
#include "ray.h"
#include "hit.h"
#include "light.h"
#include "texture.h"
class Material {
protected:
	Material() {}
public:
	virtual Vector3f shade(const Ray& ray, const Hit& hit, const PointLight& l) const=0;
	virtual bool sample(Vector3f& col, const Hit& hit, Ray& ray, Vector3f& coe, Runtime& rt) const=0;
	virtual bool need_coords() const{
		return 0;
	}
	virtual bool fix_norm() const {
		return 1;
	}
};

//0: ??, 1: COS, 2: RAW
#define SAMPLER 1
class BRDFMaterial: public Material {
public:
	const Texture diffuseColor;
	const Texture specularColor;
	float roughness;
	const Texture emissionColor;
	BRDFMaterial(const Texture& d_color = Vector3f::ZERO,
		const Texture& s_color = Vector3f::ZERO,
		float roughness = 0,
		const Texture& e_color = Vector3f::ZERO) :
		diffuseColor(d_color), specularColor(s_color),
		roughness(roughness), emissionColor(e_color) {
	}

	//cos already applied
	Vector3f BRDF(const Vector3f& in, const Vector3f& out, const Hit& hit) const {
		Vector3f L = out.normalized();
		Vector3f N = hit.norm;
		float LN = Vector3f::dot(L, N);
		if (LN <= 0) return Vector3f::ZERO;
		Vector3f R = 2 * LN * N - L, V = -in; V.normalize();
		Vector3f H = L + V; H.normalize();
		float HN = Vector3f::dot(H, N), VH = Vector3f::dot(V, H),
			VN = Vector3f::dot(V, N), LH = Vector3f::dot(L, H);
		if (VN < 0) return Vector3f::ZERO; //?
		float nhh = HN * HN;
		float D = expf((nhh - 1) / (roughness * roughness * nhh)) / (roughness * roughness * nhh * nhh);
		//		float D = roughness * roughness / (PI * td * td);
		float u = 1 - VN; //u^5
		u *= u; u *= u; u *= 1 - VN;
		Vector3f F = specularColor.get(hit.pos) + (1 - specularColor.get(hit.pos)) * u;
		float G = std::min(std::min(HN * VN / VH, HN * LN / VH) * 2, float(1));
		return LN * diffuseColor.get(hit.pos)
			+ D * F * G / (4 * VN);
	}

	Vector3f BRDF0(const Vector3f& in, const Vector3f& out, const Hit& hit) const {
		Vector3f L = out.normalized();
		Vector3f N = hit.norm;
		float LN = Vector3f::dot(L, N);
		if (LN <= 0) return Vector3f::ZERO;
		Vector3f R = 2 * LN * N - L, V = -in; V.normalize();
		Vector3f H = L + V; H.normalize();
		float HN = Vector3f::dot(H, N), VH = Vector3f::dot(V, H),
			VN = Vector3f::dot(V, N), LH = Vector3f::dot(L, H);
		if (VN < 0) return Vector3f::ZERO; //?
		float nhh = HN * HN;
		float D = expf((nhh - 1) / (roughness * roughness * nhh)) / (roughness * roughness * nhh * nhh);
		//		float D = roughness * roughness / (PI * td * td);
		float u = 1 - VN; //u^5
		u *= u; u *= u; u *= 1 - VN;
		Vector3f F = specularColor.get(hit.pos) + (1 - specularColor.get(hit.pos)) * u;
		float G = std::min(std::min(HN * VN / VH, HN * LN / VH) * 2, float(1));
		return LN * diffuseColor.get(hit.pos);
	}

	Vector3f BRDF1(const Vector3f& in, const Vector3f& out, const Hit& hit) const {
		Vector3f L = out.normalized();
		Vector3f N = hit.norm;
		float LN = Vector3f::dot(L, N);
		if (LN <= 0) return Vector3f::ZERO;
		Vector3f R = 2 * LN * N - L, V = -in; V.normalize();
		Vector3f H = L + V; H.normalize();
		float HN = Vector3f::dot(H, N), VH = Vector3f::dot(V, H),
			VN = Vector3f::dot(V, N), LH = Vector3f::dot(L, H);
		if (VN < 0) return Vector3f::ZERO; //?
		float nhh = HN * HN;
		float D = expf((nhh - 1) / (roughness * roughness * nhh)) / (roughness * roughness * nhh * nhh);
		//		float D = roughness * roughness / (PI * td * td);
		float u = 1 - VN; //u^5
		u *= u; u *= u; u *= 1 - VN;
		Vector3f F = specularColor.get(hit.pos) + (1 - specularColor.get(hit.pos)) * u;
		float G = std::min(std::min(HN * VN / VH, HN * LN / VH) * 2, float(1));
		return D * F * G / (4 * VN);
	}

	Vector3f shade(const Ray& ray, const Hit& hit, const PointLight& l) const override{
		return BRDF(ray.d, l.o - ray.pos(hit.t), hit) * l.c;
	}

	bool need_coords() const override{
		return diffuseColor.need_coord() || specularColor.need_coord() || emissionColor.need_coord();
	}
	
	bool sample(Vector3f& col, const Hit& hit, Ray& ray, Vector3f& coe, Runtime& rt) const override{
		RNG_TYPE& g = *rt.g;
//		float& davg = rt.davg[this->id];
//		float& favg = rt.favg[this->id];
		col += this->emissionColor.get(hit.pos) * coe;
		if (Vector3f::dot(hit.norm, -ray.d) < 0) return 0;
		Vector3f hitp = ray.pos(hit.t);
		Vector3f p0 = hit.norm, p1, p2;
		createCoordinateSystem(p0, p1, p2);
		float r = this->roughness;
#if SAMPLER == 0
		if (0)//g() % 2)
		{
			float cog = sqrtf(randf(g)), phi = randf(g) * PI * 2;
			float sinTheta = sqrtf(1 - cog * cog);
			float C = cog * 2;
			float y = sinTheta * cosf(phi), z = sinTheta * sinf(phi);
			Vector3f np = cog * p0 + y * p1 + z * p2;
			Vector3f H = np - ray.d; H.normalize();
			float co = Vector3f::dot(H, p0);
			float D = expf((1 - 1.0 / (co * co)) / (r * r)) / (r * r * co * co * co);
			if (D < 0) D = 0;
			coe = coe * this->BRDF0(ray.d, np, hit) / cog;
//			coe = coe * this->BRDF0(ray.d, np, hit) / cog / 2 * 2;// *((C * C) / (C * C + D * D)) * 2;
			ray = Ray(hitp, np);
		}
		else
		{
			int fail = 0;
			while (1)
			{
				float u = randf(g);
				float co = sqrtf(1 / (1 - r * r * logf(1 - u)));
				float d = expf((1 - 1.0 / (co * co)) / (r * r)) / (r * r * co * co * co);
				//			float D = d;// / co;
				float phi = randf(g) * 2 * PI, g = sqrtf(1 - co * co);
				Vector3f H = p0 * co + (p1 * cosf(phi) + p2 * sinf(phi)) * g;
				Vector3f np = H * 2 * Vector3f::dot(H, -ray.d) + ray.d; np.normalize();
				if (Vector3f::dot(np, p0) < 0)
				{
					fail++;
					if (fail >= 20)
					{
						return 0;
					}
					continue;
				}
				//			float C = Vector3f::dot(p0, np) * 2;
				d /= Vector3f::dot(np, H) * 2;
				davg = davg * 0.95 + 0.05 * d;
				favg = favg * 0.95 + 0.05 * (1 + fail);
				coe = coe * this->BRDF(ray.d, np, hit) / d * (davg/favg) / 2;
				//				coe = coe * this->BRDF1(ray.d, np, hit) / d * davg * 2;
//				coe = coe * this->BRDF1(ray.d, np, hit) * davg / d;// *2;// *((D * D) / (C * C + D * D)) * 2;
				ray = Ray(hitp, np);
				break;
			}
		}
#elif SAMPLER == 1
		float C = sqrtf(randf(g)), phi = randf(g) * PI * 2;
		float sinTheta = sqrtf(1 - C * C);
		float y = sinTheta * cosf(phi), z = sinTheta * sinf(phi);
		Vector3f np = C * p0 + y * p1 + z * p2;
		coe = coe * this->BRDF(ray.d, np, hit) / C / 2;
		ray = Ray(hitp, np);
#else
		float C = randf(g), phi = randf(g) * PI * 2;
		float sinTheta = sqrtf(1 - C * C);
		float y = sinTheta * cosf(phi), z = sinTheta * sinf(phi);
		Vector3f np = C * p0 + y * p1 + z * p2;
		coe = coe * this->BRDF(ray.d, np, hit);
		ray = Ray(hitp, np);
#endif
#undef SAMPLER
		return 1;
	}

protected:
	float clamp(float x)
	{
		if (x < 0) return 0;
		if (x > 1) return 1;
		return x;
	}
};

class MirrorMaterial : public Material {
public:
	const Texture reflectColor;
	MirrorMaterial(const Texture & reflect_color = Vector3f::ZERO) :
		reflectColor(reflect_color) {
	}

	Vector3f shade(const Ray& ray, const Hit& hit, const PointLight& l) const override {
		return Vector3f(0, 0, 0);
	}

	bool need_coords() const override {
		return reflectColor.need_coord();
	}

	bool sample(Vector3f& col, const Hit& hit, Ray& ray, Vector3f& coe, Runtime& rt) const override {
		RNG_TYPE& g = *rt.g;
		coe = coe * reflectColor.get(hit.pos);
		Vector3f np = hit.norm * 2 * Vector3f::dot(hit.norm, -ray.d) + ray.d; np.normalize();
		ray.o = ray.pos(hit.t); ray.d = np; return 1;
	}
};

class SolidGlassMaterial : public Material {
public:
	const Texture reflectColor;
	const Texture refractColor;
	float refractCoef, r0;
	SolidGlassMaterial(float coef = 1,
		const Texture& reflect_color = Vector3f::ZERO,
		const Texture& refract_color = Vector3f::ZERO) :
		refractCoef(coef),
		reflectColor(reflect_color),refractColor(refract_color) {
		r0 = (1 - refractCoef) / (1 + refractCoef); r0 *= r0;
	}

	Vector3f shade(const Ray& ray, const Hit& hit, const PointLight& l) const override {
		return Vector3f(0, 0, 0);
	}

	bool need_coords() const override{
		return reflectColor.need_coord() || refractColor.need_coord();
	}

	bool sample(Vector3f& col, const Hit& hit, Ray& ray, Vector3f& coe, Runtime& rt) const override {
		using std::cerr;
		RNG_TYPE& g = *rt.g;
		float co = Vector3f::dot(-ray.d, hit.norm);
//		std::cerr << co << "sample!"<<" "<<hit.norm[0]<<"w"<<hit.norm[1]<<"w"<<hit.norm[2]<<" "<<ray.d[0]<<"w"<<ray.d[1]<<"w"<<ray.d[2]<<"\n";
		if (co > 0) { //air->glass
			float si = sqrtf(std::max(1 - co * co, float(0)));
			float si2 = si * refractCoef;
//			cerr << "check A " << si2 << "\n";
			if (si2 <= 1) { //total reflection otherwise
				float u = 1 - co;
				float r = r0 + (1 - r0) * u * u * u * u * u;
				r = 0.25 + 0.5 * r;
//				cerr << "prob=" << r << "  " << r0 << "\n";
				if (randf(g) > r) //refract
				{
					//get refraction ray as a linear combination
					float beta = asinf(si), alpha = asinf(si2);
					//					std::cerr << "refract A " << si << "->" << si2 << "  "<<beta<<"->"<<alpha<<"\n";
					float s = beta - alpha;
					ray.o = ray.pos(hit.t);
					//					ray.d = -((-ray.d) * s + hit.norm).normalized();
					if (si != 0) //unchanged otherwise
						ray.d = (ray.d - hit.norm.normalized() * (sinf(s) / si)).normalized();
					coe = coe * refractColor.get(hit.pos) / r;
					return 1;
				}
				else coe = coe / (1 - r);
			}
		}
		else { //glass->air
			co = -co;
			float si = sqrtf(std::max(1 - co * co,float(0)));
			float si2 = si / refractCoef;
//			cerr << "check B " << si2 << " "<<ray.d[0]<<"w"<<ray.d[1]<<"w"<<ray.d[2]<<" "<<hit.t<<"\n";
//			std::cerr << si << "w" << si2 << "  "<< 1 - co * co <<"?\n";
			if (si2 <= 1) { //total reflection otherwise
					//get refraction ray as a linear combination
				float beta = asinf(si), alpha = asinf(si2);
				//					std::cerr << "refract B " << si << "->" << si2 << "  " << beta << "->" << alpha << "\n";
				float s = beta - alpha;
				Ray ray2;
				ray2.o = ray.pos(hit.t);
				if (si != 0) //unchanged otherwise
					ray2.d = (ray.d + hit.norm.normalized() * (sinf(s) / si)).normalized();
				else ray2.d = ray.d;
				float u = 1 - Vector3f::dot(hit.norm, ray2.d);
//				std::cerr << u << "?\n";
				float r = r0 + (1 - r0) * u * u * u * u * u;
				r = 0.25 + 0.5 * r;
//				cerr << "prob=" << r << "  "<<r0<<"\n";
				if (randf(g) > r) //refract
				{
					ray = ray2;
					coe = coe * refractColor.get(hit.pos) / r;
					return 1;
				}
				else coe = coe / (1 - r);
			}
		}
//		std::cerr << "reflect :(\n";
		//reflect
		coe = coe * reflectColor.get(hit.pos);
		Vector3f np = hit.norm * 2 * Vector3f::dot(hit.norm, -ray.d) + ray.d; np.normalize();
		ray.o = ray.pos(hit.t); ray.d = np; return 1;
		/*
		float co = Vector3f::dot(-ray.d, hit.norm);
		bool in = co > 0; if (!in) co = -co;
		float c = in ? (1 / refractCoef) : refractCoef;
		float cos2t = 1 - c * c * (1 - co * co);
		if (cos2t < 0) //internal
		{
			coe = coe * reflectColor.get(hit.pos);
			Vector3f np = hit.norm * 2 * Vector3f::dot(hit.norm, -ray.d) + ray.d; np.normalize();
			ray.o = ray.pos(hit.t); ray.d = np; return 1;
		}
		Vector3f rp;
		float c = 1 - (in ? co : Vector3f::dot(rp, hit.norm));
		float p = r0 + (1 - r0) * c * c * c * c * c;
		if (randf(g) < p) { //reflect
			coe = coe * reflectColor.get(hit.pos);
			Vector3f np = hit.norm * 2 * Vector3f::dot(hit.norm, -ray.d) + ray.d; np.normalize();
			ray.o = ray.pos(hit.t); ray.d = np; return 1;
		}
		else { //refract
			coe = coe * reflectColor.get(hit.pos);
			Vector3f np = hit.norm * 2 * Vector3f::dot(hit.norm, -ray.d) + ray.d; np.normalize();
			ray.o = ray.pos(hit.t); ray.d = np; return 1;
		}
		bool into = n.dot(nl) > 0;                // Ray from outside going in? 
		double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;
		if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection 
			return obj.e + f.mult(radiance(reflRay, depth, Xi));
		Vec tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
		double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
		double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
		return obj.e + f.mult(depth > 2 ? (erand48(Xi) < P ?   // Russian roulette 
			radiance(reflRay, depth, Xi) * RP : radiance(Ray(x, tdir), depth, Xi) * TP) :
			radiance(reflRay, depth, Xi) * Re + radiance(Ray(x, tdir), depth, Xi) * Tr);*/
	}
};

#endif