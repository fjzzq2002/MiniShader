#ifndef POLYSOLVER_H
#define POLYSOLVER_H
#include <vector>
#include <math.h>
#include <algorithm>
#include "utils.h"
//poor numerical stability, use with care
namespace Poly_Solver {
	using std::vector;
	using std::pair;
	using std::make_pair;
	vector<double> de(vector<double> v) {
		for (int i = 0; i + 1 < (int)v.size(); ++i)
			v[i] = v[i + 1] * (i + 1);
		if (v.size()) v.pop_back();
		return v;
	}
	vector<double> poly_solve(vector<double> v, double l, double r, double eps = 1e-8) {
		static RNG_TYPE rng;
		vector<double> dv, s;
		int retry = 0, tat=0;
		while (v.size() > 1) {
			double mx = 0;
			for (auto t : v) mx = max(mx, fabs(t));
			if (mx)
			{
				for (auto& t : v) t /= mx;
			}
			dv = de(v);
			double g = l + randf(rng) * (r - l);
			++tat;
			if (tat % 5 == 0) g = l;
			else if (tat % 5 == 1) g = r;
			for (int j = 1; j <= 30; ++j) {
				g = g - poly_calc(v, g) / poly_calc(dv, g);
				if (g != g || g > 1e10 || g < -1e10)
					break;
			}
			if (g != g || g > 1e10 || g < -1e10) ++retry;
			else if (fabs(poly_calc(v, g)) > eps) retry += 2;
			else {
				retry = 0;
				if (g >= l && g <= r) s.push_back(g);
				vector<double> nx(v.size());
				//v/(x-g)
				for (int j = (int)v.size() - 1; j >= 1; --j) {
					nx[j] = v[j]; v[j - 1] += v[j] * g;
				}
				nx.erase(nx.begin()); v = nx;
			}
			if (retry >= 14) break;
		}
		return s;
		/*
		if (v.size() == 0)
			return vector<double>();
//		v = poly_normalized(v);
		vector<double> w = poly_solve(de(v), l, r, eps, false), o;
		w.insert(w.begin(), l);
		w.push_back(r);
		for (int j = 0; j + 1 < (int)w.size(); ++j) {
			double a = w[j], b = w[j+1], fa = poly_calc(v, a), fb = poly_calc(v, b);
			if (fabs(fa) < eps) {
				o.push_back(a);
				continue;
			}
			if (fabs(fb) < eps) {
				o.push_back(b);
				continue;
			}
			if ((fa > 0) == (fb > 0))
				continue;
			for (int k = 0; k < 100 && (a!=b); ++k) {
				double m = (a + b) / 2, fm = poly_calc(v, m);
				if ((fa > 0) ^ (fm > 0)) b = m, fb = fm;
				else a = m, fa = fm;
			}
			o.push_back(a);
		}
		if (verify)
		{
			vector<double> oo;
			for (auto g : o)
				if (fabs(poly_calc(v, g)) < eps) oo.push_back(g);
			o = oo;
		}
		return o;*/
	}
	pair<double, double> poly_peak(vector<double> v, double l, double r) {
		vector<double> g = poly_solve(de(v), l, r, 1e-3); g.push_back(r);
		double mi=poly_calc(v,l), mx=mi;
		for (auto t : g)
		{
			double x = poly_calc(v, t);
			mi = min(mi, x);
			mx = max(mx, x);
		}
		return make_pair(mi, mx);
	}
}
using Poly_Solver::poly_solve;
using Poly_Solver::poly_peak;
#endif