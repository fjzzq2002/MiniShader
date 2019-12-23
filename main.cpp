#include <iostream>
#include "camera.h"
#include "group.h"
#include "sphere.h"
#include "plane.h"
#include "server.h"
#include "light.h"
#include "material.h"
#include "triangle.h"
#include "rotate_bspline.h"
#include "scene.h"
#include "runtime.h"
#include <cmath>
#include <ctime>
#include <random>
#include "ThreadPool.h"
#include "sceneloader.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
Scene scene; int w, h;

int MAX_DEP;

Vector3f tot[1000][1000];
int cnt = 0;

inline Vector3f trace(Runtime& rt, Ray ray)
{
	Vector3f col(0, 0, 0), coe(1, 1, 1);
	for (int j = 0; ; ++j)
	{
		Hit hit;
		bool isIntersect = scene.group.intersect(ray, hit, float(1e-3));
		if (!isIntersect) break;
		if(MAX_DEP<0)
		{
			if(MAX_DEP==-1)
				col += (hit.norm+Vector3f(0.5,0.5,0.5));
			else if(MAX_DEP==-2)
				col += (ray.pos(hit.t)/3);
			else
			{
				std::cerr << "unknown debug mode\n";
				exit(-1);
			}
			break;
		}
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
			if (!t) col += hit.obj->m->shade(ray, hit, *l) * coe;
		}
		if (!hit.obj->m->sample(col, hit, ray, coe, rt)) break;
		if (j >= MAX_DEP) break;
	}
	return col;
}
bool execute(Camera* camera, int x)
{
	static Runtime rt[1000];
	if (!rt[x].g)
	{
		rt[x].g = new RNG_TYPE(x);
		/*
		rt[x].davg.resize(scene.mat_num);
		rt[x].favg.resize(scene.mat_num);
		for (auto& x : rt[x].davg) x = 1;
		for (auto& x : rt[x].favg) x = 1;
		*/
	}
	for (int y = int(camera->getHeight() - 1); y >= 0; --y)
	{
		Ray camRay = camera->generateRay(x, y, *rt[x].g);
		Vector3f col = trace(rt[x], camRay);
		auto clamp = [&](float t) {if (t > 10) t = 10; if (t < 0) t = 0; if (t != t) t = 0; return t; };
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
	unsigned char* img = 0; int ls = 0;
	Camera* camera = scene.camera;
	int w = camera->getWidth(), h = camera->getHeight(), s = w * h * 3;
	if (s != ls) free(img), img = (unsigned char* )malloc(sizeof(unsigned char) * (ls=s));
	std::cerr << "shading start (" << keep << "," << cnt << ")" << std::endl;
	time_t t1 = clock();
	if (!keep)
		for (int x = 0; x < w; ++x)
			for (int y = 0; y < h; ++y)
				tot[x][y] = Vector3f::ZERO;
	std::vector<std::future<bool>> v(w);
	for (int x = 0; x < w; ++x)
		v[x]=pool->enqueue(execute, camera, x);
	for (int x = 0; x < w; ++x) v[x].get();
	for (int x = 0; x < w; ++x)
		for (int y = 0; y < h; ++y)
		{
			for (int j = 0; j < 3; ++j)
			{
				int ww = tot[x][y][j] / cnt * 255;
				if (ww < 0) ww = 0;
				if (ww > 255) ww = 255;
				img[x * 3 + y * w * 3 + j] = ww;
			}
		}
	time_t t2 = clock();
	stbi_write_bmp("output.bmp", h, w, 3, img);
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
