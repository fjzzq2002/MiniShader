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
	if(c.size()==3)
		return Vector3f(c[0], c[1], c[2]);
	if (c.size() == 2)
	{
		std::string s = c[0];
		Texture*t=new Texture(s.c_str());
		Texture g;
		g.type = TEXTURE_SCALE;
		g.source = t;
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
	else if (c["type"] == "MirrorMaterial") {
		return mat_map[d] = new MirrorMaterial(json_to_texture(c["c"]));
	}
	else assert(0);
}
template<class T>
PointLight* json_parse_light(const T& c)
{
	return new PointLight(json_to_v3(c["o"]), json_to_v3(c["c"]));
}
template<class T>
void json_parse_object(std::vector<Object*>& v, const T& c)
{
//	std::cerr << c["type"] << std::endl;
	if (c["type"] == "Plane")
		v.push_back(new Plane(json_to_v3(c["k"]), c["c"], json_parse_material(c["m"])));
	else if (c["type"] == "Sphere")
		v.push_back(new Sphere(json_to_v3(c["o"]), c["r"], json_parse_material(c["m"])));
	else if (c["type"] == "Triangle")
		v.push_back(new Triangle(json_to_v3(c["a"]),
			json_to_v3(c["b"]), json_to_v3(c["c"]), json_parse_material(c["m"])));
	else if (c["type"] == "RotateBSpline")
	{
		std::vector<Rotate_BSpline_Segment*> w = Rotate_BSpline
		(json_to_v3(c["o"]), json_to_v2_vector(c["c"]), json_parse_material(c["m"]));
		for (auto t : w) v.push_back(t);
	}
	else assert(0);
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
		scene.point_lights.push_back(json_parse_light(obj));
	scene.group.reset_obj();
}
void load_scene(Scene& scene)
{
	return load_scene_json(scene,"scenes/scene.json");
}
#endif