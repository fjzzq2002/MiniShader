#include <iostream>
#include "image.h"
#include "camera.h"
#include "group.h"
#include "sphere.h"
#include "plane.h"
#include "server.h"
#include "light.h"
#include "material.h"
#include "triangle.h"
#include "scene.h"
#include <cmath>
#include <ctime>
#include <random>
#include "ThreadPool.h"
#include "sceneloader.h"
Scene load_scene();
Scene scene; int w, h;

inline void createCoordinateSystem(const Vector3f& N, Vector3f& Nt, Vector3f& Nb)
{
	if (std::fabs(N.m_elements[0]) > std::fabs(N.m_elements[1]))
		Nt = Vector3f(N.m_elements[2], 0, -N.m_elements[0]) / sqrtf(N.m_elements[0] * N.m_elements[0] + N.m_elements[2] * N.m_elements[2]);
	else
		Nt = Vector3f(0, N.m_elements[2], -N.m_elements[1]) / sqrtf(N.m_elements[1] * N.m_elements[1] + N.m_elements[2] * N.m_elements[2]);
	Nb = Vector3f::cross(N, Nt);
}

int MAX_DEP;

Vector3f tot[1000][1000];
int cnt = 0;

#define NO_MIS
inline Vector3f trace(RNG_TYPE& g, Ray ray)
{
	Vector3f col(0, 0, 0), coe(1, 1, 1);
#ifndef NO_MIS
	bool typ = g() % 2 == 0;
#endif
	for (int j = 0; ; ++j)
	{
		Hit hit;
		bool isIntersect = scene.group.intersect(ray, hit, float(1e-3));
		if (!isIntersect) break;
		col += hit.obj->m->emissionColor * coe;
		if (Vector3f::dot(hit.norm, -ray.d) < 0) break;
		for (auto l : scene.point_lights)
		{
			Hit hit2;
			Vector3f pnt = ray.pos(hit.t);
			Ray ray2(l->o, (pnt - l->o).normalized());
			bool t = scene.group.intersect(ray2, hit2, float(1e-3));
			if (t)
			{
				pnt = ray2.pos(hit2.t) - pnt;
				t &= fabs(pnt.m_elements[0]) > 1e-3 || fabs(pnt.m_elements[1]) > 1e-3 || fabs(pnt.m_elements[2]) > 1e-3;
			}
			if (!t) col += hit.obj->m->Shade(ray, hit, *l) * coe;
		}
		if (j >= MAX_DEP) break;
		Vector3f hitp = ray.pos(hit.t);
		Vector3f p0 = hit.norm, p1, p2;
		createCoordinateSystem(p0, p1, p2);
		float r = hit.obj->m->roughness;
#ifndef NO_MIS
		if(typ)
		{
			float C = sqrtf(randf(g)), phi = randf(g) * PI * 2;
			float sinTheta = sqrtf(1 - C * C);
			float y = sinTheta * cosf(phi), z = sinTheta * sinf(phi);
			Vector3f np = C * p0 + y * p1 + z * p2;
			Vector3f H = np - ray.d; H.normalize();
			float NH = Vector3f::dot(H, p0);
			float nhh = NH * NH;
			float D = expf((nhh - 1) / (r * r * nhh)) / (r * r * nhh * nhh);
			if (D < 0) D = 0;
			coe = coe * hit.obj->m->BRDF(ray.d, np, hit) / C / 2 *((C * C) / (C * C + D * D)) * 2;
			ray = Ray(hitp, np);
		}
		else
		{
			float u = randf(g);
			float co = sqrtf(1 / (1 - r * r * logf(1 - u)));
			float nhh = co * co;
			float D = expf((nhh - 1) / (r * r * nhh)) / (r * r * nhh * nhh);
			float phi = randf(g) * 2 * PI, g = sqrtf(1 - co * co);
			Vector3f H = p0 * co + (p1 * cosf(phi) + p2 * sinf(phi)) * g;
			Vector3f np = H * 2 + ray.d; np.normalize();
			float C = Vector3f::dot(p0, np); if (C < 0) C = 0;
			coe = coe * hit.obj->m->BRDF(ray.d, np, hit) / D * ((D * D) / (C * C + D * D)) * 2;
			ray = Ray(hitp, np);
		}
#else
		float C = sqrtf(randf(g)), phi = randf(g) * PI * 2;
		float sinTheta = sqrtf(1 - C * C);
		float y = sinTheta * cosf(phi), z = sinTheta * sinf(phi);
		Vector3f np = C * p0 + y * p1 + z * p2;
		coe = coe * hit.obj->m->BRDF(ray.d, np, hit) / C / 2;
		ray = Ray(hitp, np);
#endif
	}
	return col;
}
bool execute(Camera* camera, int x)
{
	static RNG_TYPE* rng[1000];
	if (!rng[x]) rng[x] = new RNG_TYPE(x);
	for (int y = int(camera->getHeight() - 1); y >= 0; --y)
	{
		Ray camRay = camera->generateRay(x, y, *rng[x]);
		Vector3f col = trace(*rng[x], camRay);
		auto clamp = [&](float x) {if (x > 1000) x = 1000; return x; };
		col[0] = clamp(col[0]);
		col[1] = clamp(col[1]);
		col[2] = clamp(col[2]);
		tot[x][y] += col;
	}
	return 1;
}

ThreadPool *pool;

void reshade(bool keep)
{
	if (!keep) cnt = 1; else ++cnt;
	Camera* camera = scene.camera;
	std::cerr << "shading start (" << keep << "," << cnt << ")" << std::endl;
	time_t t1 = clock();
	Image renderedImg(camera->getWidth(), camera->getHeight());
	if (!keep)
		for (int x = 0; x < camera->getWidth(); ++x)
			for (int y = 0; y < camera->getHeight(); ++y)
				tot[x][y] = Vector3f::ZERO;
	std::vector<std::future<bool>> v(camera->getWidth());
	for (int x = 0; x < camera->getWidth(); ++x)
		v[x]=pool->enqueue(execute, camera, x);
	for (int x = 0; x < camera->getWidth(); ++x) v[x].get();
	for (int x = 0; x < camera->getWidth(); ++x)
		for (int y = 0; y < camera->getHeight(); ++y)
			renderedImg.SetPixel(x, camera->getHeight() - 1 - y, tot[x][y]/cnt);
	time_t t2 = clock();
	renderedImg.SaveBMP("output.bmp");
	std::cerr << "end shading " << (t2 - t1) * 1.0 / CLOCKS_PER_SEC << "s    " << 1.0 / ((t2 - t1) * 1.0 / CLOCKS_PER_SEC) << "fps\n";
	std::cerr << "time passed: " << t2 * 1.0 / CLOCKS_PER_SEC << "s\n";
}
int main(int argc,char**argv)
{
	load_scene(scene);
	w = scene.camera->width;
	h = scene.camera->height;
	std::cout << "max depth: ";
	std::cin >> MAX_DEP;
    std::cout << "ppm (0 for server, >0 for rendering only): ";
    int ppm;
    std::cin>>ppm;
	std::cout << "no. of threads: ";
	int ns;
	std::cin >> ns;
	pool = new ThreadPool(ns);
    if(ppm<=0)
    {
		PARAMLIST_TYPE param_list;
		scene.camera->dump(param_list);
        Server server(reshade, param_list);
    	server.run();
    }
    else
    {
        time_t tl=clock();
        for(int j=0;j<ppm;++j)
        {
            std::cerr<<"frame "<<j<<std::endl;
			reshade(j!=0);
        }
        std::cerr<<"done. used "<<clock()*1.0/CLOCKS_PER_SEC<<"s\n";
    }
}
