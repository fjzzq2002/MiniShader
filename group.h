#ifndef GROUP_H
#define GROUP_H
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "utils.h"
#include "ray.h"
#include "object.h"
using std::cerr;
using std::endl;
class Group{
public:
	std::vector<Object*> obj;
	std::vector<int> ch;
	std::vector<BoundingBox> box,t1,t2;
	std::vector<int> rs;
	int bn;
	int build(int* v,int vn,int c=0)
	{
//		cerr << vn << "!" << v[0] << "!!\n";
//		for (int j = 0; j < vn; ++j) cerr << v[j] << "|"; cerr << "EZ\n";
		if (vn == 1) return v[0];
		int p = bn++;
		std::sort(v,v+vn, [&](int p, int q) {return box[p].a[c] + box[p].b[c] < box[q].a[c] + box[q].b[c]; });
		t1[0] = BoundingBox::EMPTY;
		for (int i = 0; i < vn; ++i)
			t1[i+1] = t1[i] | box[v[i]];
		t2[vn] = BoundingBox::EMPTY;
		for (int i = vn - 1; i >= 0; --i)
			t2[i] = t2[i + 1] | box[v[i]];
		int I=vn/2;
		float mxx = std::max(t1[I].area(),t2[I].area());
		for (int i = 1; i < vn; ++i)
		{
			float ma = std::max(t1[i].area(), t2[i].area());
			if (ma < mxx) mxx = ma, I = i;
		}
		ch[p * 2] = build(v, I, (c+1)%3);
//		cerr << p << "FIRSTHALF ED\n";
		ch[p * 2 + 1] = build(v + I, vn - I,(c+1)%3);
		box[p] = box[ch[p * 2]] | box[ch[p * 2 + 1]];
		return p;
	}
	void reset_obj() {
		int n = obj.size();
		if (!n) return;
		t1.resize(n+1);
		t2.resize(n+1);
		rs.clear();
		ch.resize(n*4);
		box.resize(n*2);
		bn = n;
		std::vector<int> vec;
		for (int i = 0; i < n; ++i)
			box[i] = obj[i]->getBound(),
			vec.push_back(i);
		sort(vec.begin(), vec.end(), [&](int p, int q) {return box[p].area() < box[q].area(); });
		int* cv = (int*)malloc(sizeof(int) * vec.size());
		while (!vec.empty())
		{
			int cn = 0;
			float u = box[vec.back()].area();
			while (!vec.empty()&&box[vec.back()].area() * 4 >= u)
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
		if (!box[id].intersect(r, tmin, h.t + 1e-3)) return 0;
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
		if (ok && h.obj->m->fix_norm() && Vector3f::dot(h.norm, -r.d) < 0)
			h.norm = -h.norm;
		return ok;
#endif
	}

	int getGroupSize() {
		return obj.size();
	}
};

#endif
