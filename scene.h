#ifndef SCENE_H
#define SCENE_H

#include "utils.h"
#include <vector>
#include "group.h"
#include "light.h"
#include "camera.h"

//only a wrapper currently
struct Scene {
	Group group;
	std::vector<PointLight*> point_lights;
	Camera* camera;
	Scene() {}
};
#endif