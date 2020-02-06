#ifndef SCENELOADER_H
#define SCENELOADER_H
#include <iostream>
#include "camera.h"
#include "group.h"
#include "sphere.h"
#include "plane.h"
#include "server.h"
#include "light.h"
#include "texture.h"
#include "material.h"
#include "triangle.h"
#include "rotate_bspline.h"
#include "scene.h"
#include <cmath>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
template<class T>
Vector3f json_to_v3(const T& c)
{
	return Vector3f(c[0], c[1], c[2]);
}
template<class T>
Vector2f json_to_v2(const T& c)
{
	return Vector2f(c[0], c[1]);
}
template<class T>
std::vector<Vector2f> json_to_v2_vector(const T& c)
{
	std::vector<Vector2f> v;
	for (auto x : c) v.push_back(json_to_v2(x));
	return v;
}
template<class T>
Texture json_to_texture(const T& c)
{
	if (c.count("type")&&c["type"]=="ImageTexture")
	{
		std::string s = c["s"];
		Texture* t = new Texture(s.c_str());
		Texture g;
		g.type = TEXTURE_SCALE;
		g.source = t; g.empty = false;
		Vector2f o = json_to_v2(c["o"]);
		float x = c["k"][0], y = c["k"][1];
		g.scale = [=](Vector2f p) {return Vector2f(p[0] * x, p[1] * y)+o; };
		//		std::cerr << "hello scale: " << c[1][0] << "," << c[1][1] << "\n";
		return g;
	}
	if(c.size()==3)
		return Vector3f(c[0], c[1], c[2]);
	if (c.size() == 2)
	{
		std::string s = c[0];
		Texture* t = new Texture(s.c_str());
		Texture g;
		g.type = TEXTURE_SCALE;
		g.source = t; g.empty = false;
		float x = c[1][0], y = c[1][1];
		g.scale = [=](Vector2f p) {return Vector2f(p[0] * x, p[1] * y); };
//		std::cerr << "hello scale: " << c[1][0] << "," << c[1][1] << "\n";
		return g;
	}
	std::string s = c;
	return Texture(s.c_str());
}
template<class T>
Camera* json_parse_camera(const T& c)
{
	if(c["type"]=="PerspectiveCamera")
		return new PerspectiveCamera(json_to_v3(c["o"]), json_to_v3(c["d"]), json_to_v3(c["u"]),
			c["w"], c["h"], c["f"], c["b"]);
	if(c["type"]=="ThinLensCamera")
		return new ThinLensCamera(json_to_v3(c["o"]), json_to_v3(c["d"]), json_to_v3(c["u"]),
			c["w"], c["h"], c["f"], c["l"], c["r"]);
	return 0;
}
template<class T>
Material* json_parse_material(const T& c)
{
	static std::map<std::string, Material*> mat_map;
	std::string d = c.dump();
	if (mat_map.count(d)) return mat_map[d];
	std::cerr << "new mat: " << d << std::endl;
	if(c["type"]=="BRDFMaterial") {
		return mat_map[d] = new BRDFMaterial(json_to_texture(c["dc"]), json_to_texture(c["sc"]), c["r"], json_to_texture(c["ec"]));
	}
	else if (c["type"] == "SolidGlassMaterial") {
		return mat_map[d] = new SolidGlassMaterial(float(c["n"]), json_to_texture(c["reflectc"]), json_to_texture(c["refractc"]));
	}
	else if (c["type"] == "GlassMaterial") {
		return mat_map[d] = new GlassMaterial(float(c["n"]), json_to_texture(c["reflectc"]), json_to_texture(c["refractc"]), float(c["t"]));
	}
	else if (c["type"] == "MirrorMaterial") {
		return mat_map[d] = new MirrorMaterial(json_to_texture(c["c"]));
	}
	else assert(0);
}
template<class T>
void json_parse_light(Scene& s,const T& c)
{
	if(c["type"]=="PointLight")
		s.point_lights.push_back(new PointLight(json_to_v3(c["o"]), json_to_v3(c["c"])));
	else if(c["type"]=="PointLightDecay")
		s.point_lights_decay.push_back(new PointLight(json_to_v3(c["o"]), json_to_v3(c["c"])));
}
int cntt = 0;
template<class T>
void json_parse_object(std::vector<Object*>& v, const T& c)
{
	//	std::cerr << c["type"] << std::endl;
	if (c["type"] == "Plane")
		v.push_back(new Plane(json_to_v3(c["k"]), c["c"], json_parse_material(c["m"])));
	else if (c["type"] == "Sphere")
		v.push_back(new Sphere(json_to_v3(c["o"]), c["r"], json_parse_material(c["m"])));
	else if (c["type"] == "Triangle") {
		++cntt;
		v.push_back(new Triangle(json_to_v3(c["a"]),
			json_to_v3(c["b"]), json_to_v3(c["c"]), json_parse_material(c["m"])));
	}
	else if (c["type"] == "NormTriangle") {
		++cntt;
		v.push_back(new Triangle(json_to_v3(c["a"]),
			json_to_v3(c["b"]), json_to_v3(c["c"]),
			json_to_v3(c["p"]), json_to_v3(c["q"]),
			json_to_v3(c["r"]), json_parse_material(c["m"])));
	}
	else if (c["type"] == "RotateBSpline")
	{
		std::vector<Rotate_BSpline_Segment*> w = Rotate_BSpline
		(json_to_v3(c["o"]), json_to_v2_vector(c["c"]), json_parse_material(c["m"]));
		for (auto t : w) v.push_back(t);
	}
	else if (c["type"] == "HeightField")
	{
		int n = c["y"].size() - 1, m = c["y"][0].size() - 1;
		Vector2f p0 = json_to_v2(c["p"]);
		double w = c["w"], h = c["h"];
		w /= m; h /= n;
		vector<vector<Vector3f>> vec;
		RNG_TYPE rng;
		vec.resize(n + 1);
		for (int i = 0; i <= n; ++i)
			for (int j = 0; j <= m; ++j)
				vec[i].push_back(Vector3f(p0[0] + (i + (randf(rng) - 0.5) / 2) * h,
				(float)c["y"][i][j], p0[1] + (j + (randf(rng) - 0.5) / 2) * w));
		Material* mat = json_parse_material(c["m"]);
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
			{
				Vector3f o = (vec[i][j] + vec[i + 1][j + 1] + vec[i][j + 1] + vec[i + 1][j]) / 4;
				v.push_back(new Triangle(vec[i][j], vec[i + 1][j], o, mat));
				v.push_back(new Triangle(vec[i + 1][j], vec[i + 1][j + 1], o, mat));
				v.push_back(new Triangle(vec[i + 1][j + 1], vec[i][j + 1], o, mat));
				v.push_back(new Triangle(vec[i][j + 1], vec[i][j], o, mat));
				/*
				if (rand() & 1)
				{
					v.push_back(new Triangle(vec[i][j], vec[i + 1][j], vec[i][j + 1], mat));
					v.push_back(new Triangle(vec[i + 1][j], vec[i + 1][j + 1], vec[i][j + 1], mat));
				}
				else
				{
					v.push_back(new Triangle(vec[i][j], vec[i + 1][j], vec[i + 1][j + 1], mat));
					v.push_back(new Triangle(vec[i][j], vec[i + 1][j + 1], vec[i][j + 1], mat));
				}
				*/
			}
	}
	else if (c["type"] == "TextNormTriangle") {
		v.push_back(new Triangle(json_to_v3(c["a"]),
			json_to_v3(c["b"]), json_to_v3(c["c"]),
			json_to_v2(c["u"]), json_to_v2(c["v"]),
			json_to_v2(c["w"]),
			json_to_v3(c["p"]), json_to_v3(c["q"]),
			json_to_v3(c["r"]), json_parse_material(c["m"])));
	}
	else {
		cerr << "WTF\n";
		assert(0);
	}
}
void load_scene_json(Scene& scene,std::string json)
{
	std::ifstream t(json);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string tmp = buffer.str();
	auto u = json::parse(tmp);
	scene.camera = json_parse_camera(u["cam"]);
	for (auto obj : u["obj"])
		json_parse_object(scene.group.obj, obj);
	for (auto obj : u["light"])
		json_parse_light(scene,obj);
	for (auto obj : scene.group.obj)
		if (obj->remit==1)
			scene.triangle_lights.push_back((Triangle*)obj),
			scene.lights_area+=((Triangle*)obj)->area;
	std::cerr << cntt << "|"<<scene.point_lights.size() << ","<<scene.point_lights_decay.size()<<","<<scene.triangle_lights.size()<<"\n";
	scene.group.reset_obj();
}
void load_scene(Scene& scene)
{
	return load_scene_json(scene,"scenes/scene.json");
}
#endif