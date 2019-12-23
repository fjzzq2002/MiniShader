#ifndef ROTATE_BSPLINE_H
#define ROTATE_BSPLINE_H
#include "utils.h"
#include "object.h"
#include "polysolver.h"
#include <vector>
//currently only uniform bspline objects rotating through (X,Z) is supported.
struct Rotate_BSpline_Segment : public Object {
	//fill in these properties when constructing
	Vector3f o;
	std::vector<double> px, py, nx, ny;
	double l, r;

	Rotate_BSpline_Segment(Material* m) :Object(m) { }

	bool intersect(const Ray& ray, Hit& h, float tmin) override {
		if (!this->getBound().intersect(ray, tmin, h.t + 1e-3)) return 0;
		//(ray.o+ray.d*t-o)_y=py(u)
		//((ray.o+ray.d*t-o)_x)^2+((ray.o+ray.d*t-o)_z)^2-px(u)^2=0
		//t=(py(u)+Y(-ray.o+o))/Y(ray.d)
		if (fabs(ray.d[1]) < 1e-5) { //almost pararrel
			//py(u)=(ray.o-o)_y
			std::vector<double> f = py;
			if (!f.size()) f.push_back(0);
			f[0]+=(o - ray.o)[1];
			std::vector<double> oo = poly_solve(f, l, r, 1e-7);
			bool suc = 0;
			for (auto u : oo) {
				if (u<l || u>r) continue;
				double k1 = ray.d[0], b1 = (ray.o - o)[0], k2 = ray.d[2], b2 = (ray.o - o)[2], b=poly_calc(px,u);
				b = -b * b;
				//(k1*t+b1)^2+(k2*t+b2)^2+b=0
				double A = k1 * k1 + k2 * k2, B = 2 * k1 * b1 + 2 * k2 * b2, C = b1 * b1 + b2 * b2 + b;
				std::vector<double> og;
				double dt = B * B - 4 * A * C;
				if (dt >= 0) {
					dt = sqrt(dt);
					og.push_back((-B - dt) / (2 * A));
					og.push_back((-B + dt) / (2 * A));
				}
				for(auto g:og)
				if (g >= tmin && g < h.t) {
					double gx = poly_calc(px, u);
					Vector3f f = ray.pos(g) - o;
					double l = f[0] * f[0] + f[2] * f[2]; l -= gx * gx;
					if (fabs(l) > 1e-3) continue;
					h.t = g; h.obj = this;
					Vector3f pos = ray.o + ray.d * h.t - o,
						pyy = Vector3f(0, 1, 0),
						pxx = Vector3f(pos[0], 0, pos[2]).normalized();
//					double X = poly_calc(px, u), Y = poly_calc(py, u);
//					cerr << g << "," << u << "|" << X * X << "," << pos[0] * pos[0] + pos[2] * pos[2] << "|" << Y << "," << pos[1] << "\n";
					h.norm = pxx * poly_calc(nx, u) + pyy * poly_calc(ny, u);
					h.norm.normalize();
					if (Vector3f::dot(h.norm, -ray.d) < 0) h.norm = -h.norm;
					//todo: add pos, too lazy atm
					h.pos = Vector2f::ZERO; suc = 1;
				}
			}
			return suc;
		}
		float b1 = (-ray.o + o)[1], k1 = 1.0 / ray.d[1];
		//t=(py(u)+b1)*k1
		std::vector<double> t = py;
		if (!t.size()) t.push_back(0);
		t[0] += b1;
		for (auto& u : t) u *= k1;
		std::vector<double> x = t, z = t;
		for (auto& u : x) u *= ray.d[0];
		x[0] += (ray.o - o)[0];
		for (auto& u : z) u *= ray.d[2];
		z[0] += (ray.o - o)[2];
		std::vector<double> f = poly_minus(poly_add(poly_mul(x, x), poly_mul(z, z)), poly_mul(px, px));
		std::vector<double> oo = poly_solve(f, l, r, 1e-7);
//		std::cerr << "good!" << f.size() << "w" << oo.size() << "!";
//		for (auto c : oo) std::cerr << c << "|"<<poly_calc(px,c)<<"|"<<poly_calc(f,c)<<","; std::cerr << "\n";
		bool suc = 0;
		for (auto u : oo) {
			if (u<l || u>r) continue;
			double g = poly_calc(py, u);
			g = (g + b1) * k1;
			if (g >= tmin && g < h.t) {
				double gx = poly_calc(px, u);
				Vector3f f = ray.pos(g) - o;
				double l = f[0] * f[0] + f[2] * f[2]; l -= gx * gx;
				if (fabs(l) > 1e-4) continue;
				h.t = g; h.obj = this;
				Vector3f pos = ray.o + ray.d * h.t - o,
					pyy = Vector3f(0, 1, 0),
					pxx = Vector3f(pos[0], 0, pos[2]).normalized();
//				double X = poly_calc(px, u), Y = poly_calc(py, u);
//				cerr << g << "," << u << "|" << X * X << "," << pos[0] * pos[0] + pos[2] * pos[2] << "|" << Y << "," << pos[1] << "\n";
//				cerr<<pos[1]<<" "<<pos[0]*pos[0]+pos[2]*pos[2]<<"  "<<poly_calc(px,u)<<" "<<
				h.norm = pxx * poly_calc(nx, u) + pyy * poly_calc(ny, u);
				h.norm.normalize();
				if (Vector3f::dot(h.norm, -ray.d)<0) h.norm = -h.norm;
				//todo: add pos, too lazy atm
				h.pos = Vector2f::ZERO; suc = 1;
			}
		}
		return suc;
	}
	BoundingBox b=BoundingBox::EMPTY;
	//dirty but sorta ok
	BoundingBox getBound() override
	{
		if (b.size() > 0) return b;
//		std::cerr << "BOUND\n";
		std::pair<double, double> bx = poly_peak(px, l, r);
		double ux = max(fabs(bx.first), fabs(bx.second));
		std::pair<double, double> by = poly_peak(py, l, r);
//		std::cerr << l << "~" << r << "|";
//		std::cerr << o[0] - ux << "," << o[1] + by.first << "," << o[2] - ux << " " << o[0] + ux << "," << o[1] + by.second << "," << o[2] + ux << "\n";
		return b = BoundingBox(Vector3f(o[0] - ux, o[1] + by.first, o[2] - ux), Vector3f(o[0] + ux, o[1] + by.second, o[2] + ux));
	}
};
std::vector<Rotate_BSpline_Segment*> Rotate_BSpline(Vector3f o, std::vector<Vector2f> controls, Material* m, int K=3) {
//	cout << o[0] << ',' << o[1] << "," << o[2] << " | " << controls.size() << "  |  " << m<<" " << K << "?\n";
//	for (auto t : controls) cout << t[0] << "," << t[1] << " "; cout << "?\n";
	using std::vector;
	int N = int(controls.size()) - 1;
	auto tt = [&](int w) {return w * 1.0 / (N + K + 1); };
	vector<vector<Vector2f>> f(N + K + 1);
	std::vector<Rotate_BSpline_Segment*> output;
	for (int g = K; g <= N; ++g) {
		float l = tt(g), r = tt(g + 1);
		for (int j = 0; j < N + K + 1; ++j)
		{
			f[j].clear();
			if (g == j) f[j].push_back(1);
		}
		vector<Vector2f> t;
		for (int x = 1; x <= K; ++x)
		{
			if (x == K)
			{
				for (int j = 0; j < N + K + 1 - x; ++j)
					t = poly_add(t, poly_mul(poly_minus(
						poly_mul(f[j], 1.0 / (tt(j + x) - tt(j))), 
						poly_mul(f[j + 1], 1.0 / (tt(j + x + 1) - tt(j + 1)))), 
						controls[j] * x));
			}
			for (int j = 0; j < N + K + 1 - x; ++j)
				f[j] = poly_add(poly_mul(f[j], poly_mul(
					std::vector<Vector2f>{ Vector2f(-tt(j),-tt(j)),Vector2f(1,1) }, 1.0 / (tt(j + x) - tt(j)))),
				poly_mul(f[j + 1], poly_mul(std::vector<Vector2f>{ 
					Vector2f(tt(j + x + 1), tt(j + x + 1)),Vector2f(-1,-1) }, 1.0 / (tt(j + x + 1) - tt(j + 1)))));
//				f[j] = (g - tt(j)) / (tt(j + x) - tt(j)) * f[j] + (tt(j + x + 1) - g) / (tt(j + x + 1) - tt(j + 1)) * f[j + 1];
		}
		vector<Vector2f> v;
		for (int j = 0; j <= N; ++j)
			v = poly_add(v, poly_mul(f[j], controls[j]));
		auto gx = [](const vector<Vector2f>& t) {
			vector<double> s;
			for (auto g : t)
				s.push_back(g[0]);
			return s;
		};
		auto gy = [](const vector<Vector2f>& t) {
			vector<double> s;
			for (auto g : t)
				s.push_back(g[1]);
			return s;
		};
		Rotate_BSpline_Segment *s_p=new Rotate_BSpline_Segment(m),&s=*s_p;
		s.o = o; s.px = gx(v); s.py = gy(v);
		s.nx = poly_neg(gy(t)); s.ny = gx(t);
		s.l = l; s.r = r; output.push_back(s_p);
	}
//	cerr << K << "," << N << " " << output.size() << "?\n";
	return output;
}
#endif