#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "utils.h"
#include "group.h"
#include "light.h"
#include "camera.h"

//only a wrapper currently
struct Scene {
	Group group;
	std::vector<PointLight*> point_lights;
	std::vector<PointLight*> point_lights_decay;
	std::vector<Triangle*> triangle_lights;
	float lights_area;
	Camera* camera;
	Scene() {}
};
#endif