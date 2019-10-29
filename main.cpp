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
std::string to_str(const PerspectiveCamera& cam)
{
	std::vector<float> vec;
	vec.push_back(cam.center[0]);
	vec.push_back(cam.center[1]);
	vec.push_back(cam.center[2]);
	vec.push_back(cam.direction[0]);
	vec.push_back(cam.direction[1]);
	vec.push_back(cam.direction[2]);
	vec.push_back(cam.up[0]);
	vec.push_back(cam.up[1]);
	vec.push_back(cam.up[2]);
	vec.push_back(cam.dist);
	std::string out = "init ";
	for (auto v : vec)
		out += std::to_string(v) + ",";
	out.pop_back();
	return out;
}

template<class T>
inline float randf(T& g)
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

int MAX_DEP;

Vector3f tot[1000][1000];
int cnt = 0;

template<class T>
inline Vector3f trace(T& g, Ray ray)
{
	Vector3f col(0, 0, 0), coe(1, 1, 1);
	bool typ = g() % 2 == 0;
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
	}
	return col;
}
bool execute(const PerspectiveCamera& camera, int x)
{
	static unsigned cnt[1000];
	std::minstd_rand0 g((cnt[x] += camera.getWidth()) + x);
	for (int y = int(camera.getHeight() - 1); y >= 0; --y)
	{
		Ray camRay = camera.generateRay(Vector2f(x - randf(g) + 0.5, y - randf(g) + 0.5));
		Vector3f col = trace(g, camRay);
		auto clamp = [&](float x) {if (x > 1000) x = 1000; return x; };
		col[0] = clamp(col[0]);
		col[1] = clamp(col[1]);
		col[2] = clamp(col[2]);
		tot[x][y] += col;
	}
	return 1;
}

Vector3f tx[1005][1005],tg[1005][1005],tu[1005][1005];

ThreadPool *pool;

bool SILENT,NOSAVE;
void reshade(Vector3f o, Vector3f d, Vector3f u, float x, bool keep)
{
	if (!keep) cnt = 1; else ++cnt;
	if(!SILENT) std::cerr << "shading start (" << keep << "," << cnt << ")" << std::endl;
	time_t t1 = clock();
	PerspectiveCamera camera(o, d, u, w, h, x, 0);
	Image renderedImg(camera.getWidth(), camera.getHeight());
	for (int x = 0; x < camera.getWidth(); ++x)
		for (int y = 0; y < camera.getHeight(); ++y)
			tot[x][y] = Vector3f::ZERO;
	for (int x = 0; x < camera.getWidth(); ++x)
	if (!keep)
		for (int x = 0; x < camera.getWidth(); ++x)
			for (int y = 0; y < camera.getHeight(); ++y)
				tx[x][y] = Vector3f::ZERO;
	std::vector<std::future<bool>> v(camera.getWidth());
	for (int x = 0; x < camera.getWidth(); ++x)
		v[x]=pool->enqueue(execute, camera, x);
	for (int x = 0; x < camera.getWidth(); ++x) v[x].get();
	//todo: denoise?
	for (int x = 0; x < camera.getWidth(); ++x)
		for (int y = 0; y < camera.getHeight(); ++y)
		{
			for(int k=0;k<3;++k)
				if (tot[x][y][k] > 20) tot[x][y][k] = 20;
			tx[x][y] += tot[x][y];
		}
	for (int x = 0; x < camera.getWidth(); ++x)
		for (int y = 0; y < camera.getHeight(); ++y)
			renderedImg.SetPixel(x, camera.getHeight() - 1 - y, tx[x][y]/cnt);
	time_t t2 = clock();
	if(!NOSAVE) renderedImg.SaveBMP("test.bmp");
	if(!SILENT) std::cerr << "end shading " << (t2 - t1) * 1.0 / CLOCKS_PER_SEC << "s    " << 1.0 / ((t2 - t1) * 1.0 / CLOCKS_PER_SEC) << "fps\n";
	if(!SILENT) std::cerr << "time passed: " << t2 * 1.0 / CLOCKS_PER_SEC << "s\n";
}
int main(int argc,char**argv)
{
	load_scene(scene);
	w = scene.camera->width;
	h = scene.camera->height;
	std::cout << "max depth: ";
	std::cin >> MAX_DEP;
    std::cout << "ppm (<=0 for server): ";
    int ppm;
    std::cin>>ppm;
	std::cout << "no. of threads: ";
	int ns;
	std::cin >> ns;
	pool = new ThreadPool(ns);
    if(ppm<=0)
    {
        std::string cam_pos = to_str(*scene.camera);
        std::cerr << cam_pos << "\n";
        Server server(reshade, cam_pos);
    	server.run();
    }
    else
    {
        time_t tl=clock();
        for(int j=0;j<ppm;++j)
        {
            std::cerr<<"frame "<<j<<std::endl;// SILENT=1;
			if (clock() - tl > CLOCKS_PER_SEC * 3||j==ppm-1)
			{
				tl = clock();
				std::cerr << "saving\n";
				NOSAVE = 0;
			}
			else NOSAVE = 1;
			reshade(scene.camera->center,scene.camera->direction,scene.camera->up,scene.camera->dist,(j!=0));
        }
        std::cerr<<"done. used "<<clock()*1.0/CLOCKS_PER_SEC<<"s\n";
    }
}
