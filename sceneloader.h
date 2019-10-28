#ifndef SCENELOADER_H
#define SCENELOADER_H
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
#include <sstream>
#include <fstream>
#include <map>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
template<class T>
Vector3f json_to_v3(const T& c)
{
	return Vector3f(c[0], c[1], c[2]);
}
template<class T>
PerspectiveCamera* json_parse_camera(const T& c)
{
	return new PerspectiveCamera(json_to_v3(c["o"]), json_to_v3(c["d"]), json_to_v3(c["u"]),
		c["w"], c["h"], c["f"], c["b"]);
}
template<class T>
Material* json_parse_material(const T& c)
{
	static std::map<std::string, Material*> mat_map;
	std::string d = c.dump();
	if (mat_map.count(d)) return mat_map[d];
	std::cerr << "new mat: " << d << std::endl;
	return mat_map[d] = new Material(json_to_v3(c["dc"]), json_to_v3(c["sc"]), c["r"], json_to_v3(c["ec"]));
}
template<class T>
PointLight* json_parse_light(const T& c)
{
	return new PointLight(json_to_v3(c["o"]), json_to_v3(c["c"]));
}
template<class T>
Object* json_parse_object(const T& c)
{
	std::cerr << c["class"] << std::endl;
	if (c["class"] == "Plane")
		return new Plane(json_to_v3(c["k"]), c["c"], json_parse_material(c["m"]));
	if (c["class"] == "Sphere")
		return new Sphere(json_to_v3(c["o"]), c["r"], json_parse_material(c["m"]));
	if (c["class"] == "Triangle")
		return new Triangle(json_to_v3(c["a"]),
			json_to_v3(c["b"]), json_to_v3(c["c"]), json_parse_material(c["m"]));
	throw "invalid object";
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
		scene.group.obj.push_back(json_parse_object(obj));
	for (auto obj : u["light"])
		scene.point_lights.push_back(json_parse_light(obj));
	scene.group.reset_obj();
}
void load_scene(Scene& scene)
{
	return load_scene_json(scene,"scenes/scene.json");
}
#endif