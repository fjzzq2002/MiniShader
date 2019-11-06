#ifndef GROUP_H
#define GROUP_H
#include <cassert>
#include "utils.h"
#include "ray.h"
#include "object.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "memory.h"
using std::cerr;
using std::endl;
class Group{
public:
	std::vector<Object*> obj;
	std::vector<int> ch;
	std::vector<BoundingBox> box;
	std::vector<int> rs;
	int bn;
	int build(int* v,int vn,int c=0)
	{
//		cerr << vn << "!" << v[0] << "!!\n";
//		for (int j = 0; j < vn; ++j) cerr << v[j] << "|"; cerr << "EZ\n";
		if (vn == 1) return v[0];
		int p = bn++;
		std::nth_element(v,v+vn/2,v+vn, [&](int p, int q) {return box[p].a[c] + box[p].b[c] < box[q].a[c] + box[q].b[c]; });
		ch[p * 2] = build(v, vn / 2,(c+1)%3);
//		cerr << p << "FIRSTHALF ED\n";
		ch[p * 2 + 1] = build(v + vn / 2, vn - vn / 2,(c+1)%3);
		box[p] = box[ch[p * 2]] | box[ch[p * 2 + 1]];
		return p;
	}
	void reset_obj() {
		int n = obj.size();
		if (!n) return;
		rs.clear();
		ch.resize(n*4);
		box.resize(n*2);
		bn = n;
		std::vector<int> vec;
		for (int i = 0; i < n; ++i)
			box[i] = obj[i]->getBound(),
			vec.push_back(i);
		sort(vec.begin(), vec.end(), [&](int p, int q) {return box[p].size() < box[q].size(); });
		int* cv = (int*)malloc(sizeof(int) * vec.size());
		while (!vec.empty())
		{
			int cn = 0;
			float u = box[vec.back()].size();
			while (!vec.empty()&&box[vec.back()].size() * 8 >= u)
				cv[cn++]=vec.back(), vec.pop_back();
			rs.push_back(build(cv, cn));
		}
		free(cv);
	}
	Group() {
		//reset_obj();
	}
	Group(const std::vector<Object*>& vec) {
		obj = vec;
		reset_obj();
	}
	bool intersect(int id, const Ray& r, Hit& h, float tmin) {
		if (id < (int)obj.size())
			return obj[id]->intersect(r, h, tmin);
		if (!box[id].intersect(r,tmin,h.t+1e-3)) return 0;
		bool ok = 0;
		ok |= intersect(ch[id*2], r, h, tmin);
		ok |= intersect(ch[id*2+1], r, h, tmin);
		return ok;
	}
//#define NOKD
	bool intersect(const Ray& r, Hit& h, float tmin) {
#ifdef NOKD
		bool ok = 0;
		for (auto o : obj)
			if (o) ok |= o->intersect(r, h, tmin);
		return ok;
#else
		if (obj.empty()) return 0;
//		std::cerr << "INTERSECT TEST:" << obj.size() << "\n";
		bool ok = 0;
		for (auto g : rs)
			ok |= intersect(g, r, h, tmin);
		return ok;
#endif
	}

	int getGroupSize() {
		return obj.size();
	}
};

#endif
