#ifndef TEXTURE_H
#define TEXTURE_H
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "hdrloader.h"
#include <iostream>
#include <functional>
#include "utils.h"
enum TEXTURE_TYPE {TEXTURE_COLOR,TEXTURE_SCALE,TEXTURE_RECT};
class Texture {
public:
	TEXTURE_TYPE type;
	Texture* source;
	Vector3f color;
	std::function<Vector2f(Vector2f)> scale;
	int image_w, image_h, image_c;
	unsigned char* image_buff_stbi;
	float* hdr_buff;
	bool empty = false;
	bool is_empty() const { return empty; }
	Texture() :type(TEXTURE_COLOR), color(Vector3f::ZERO) {
		empty = true;
	}
	Texture(Vector3f c) :type(TEXTURE_COLOR), color(c) {
		if (c == Vector3f::ZERO) empty = true;
	}
	Texture(const char* img) :type(TEXTURE_RECT)
	{
		std::string img_ = img;
		if (img_.substr(img_.size() - 4) == ".hdr") {
			image_buff_stbi = nullptr;
			HDRLoaderResult result;
			bool ret = HDRLoader::load(img, result);
			assert(ret);
			hdr_buff = new float[result.width / 2 * result.height / 2 * 3];
			image_w = result.width/2;
			image_h = result.height/2;
			image_c = 3;
			for(int i=0;i<image_w;++i)
				for(int j=0;j<image_h;++j)
					for (int k = 0; k < 3; ++k)
					{
						float s = 0;
						for (int w = 0; w < 2; ++w)
							for (int g = 0; g < 2; ++g)
								s += result.cols[(i * 2 + w) * result.height * 3 + (j * 2 + g) * 3 + k];
						hdr_buff[i * image_h * 3 + j * 3 + k] = powf(s / 4, 0.1);
					}
		}
		else {
			hdr_buff = nullptr;
			image_buff_stbi = stbi_load(img, &image_w, &image_h, &image_c, 0);
		}
		std::cerr << "loaded image " << img << ": " << image_w << "x" << image_h << "x" << image_c << "\n";
	}
	Vector3f get(Vector2f x) const
	{
		switch (type)
		{
		case TEXTURE_COLOR: return color;
		case TEXTURE_SCALE:
			/*
			std::cerr << source << "!!\n";
			std::cerr << x[0] << "," << x[1] << "\n";
			std::cerr << scale(x)[0] << "|" << scale(x)[1] << "\n";*/
			return source->get(scale(x));
		case TEXTURE_RECT:
			auto pix = [&](int a, int b) {
				if (a < 0 || b < 0 || a >= image_w || b >= image_h)
					return Vector3f::ZERO;
				Vector3f g;
				if (image_buff_stbi != nullptr) {
					for (int x = 0; x < 3; ++x)
						g[x] = image_buff_stbi[a * image_c + b * image_w * image_c + x] / 255.0;
				}
				else {
					for (int x = 0; x < 3; ++x)
						g[x] = hdr_buff[b * image_c + a * image_h * image_c + x];// 0.8);
				}
				return g;
			};
			float a = clamp(x[0]-floor(x[0])) * (image_w - 1), b = clamp(x[1]-floor(x[1])) * (image_h - 1);
			//return pix(int(a + 0.5), int(b + 0.5));
			int l = a, r = b; float p = a - l, q = b - r;
			return pix(l, r) * (1 - p) * (1 - q) + pix(l, r + 1) * (1 - p) * q + pix(l + 1, r) * p * (1 - q) + pix(l + 1, r + 1) * p * q;
		}
	}
	inline bool need_coord() const {
		return type != TEXTURE_COLOR;
	}
	float clamp(float x) const
	{
		if (x < 0) return 0;
		if (x > 1) return 1;
		return x;
	}
};
#endif