#include "tgaimage.h"
#include "model.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;

void line(int x0, int x1, int y0, int y1, TGAImage &image, TGAColor color) 
{
	bool steep = false;
	if (std::abs(y1 - y0) > std::abs(x1 - x0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dy = y1 - y0;
	int dx = x1 - x0;
	int D = std::abs(dy)*2 - dx; 
	int y = y0;

#ifndef __DEBUG
	std::cerr << std::endl;
#endif

	if (steep) 
		for (int x = x0; x <= x1; x += 1)
		{
			image.set(y, x, color); 
			
			if (D > 0) {
				y += (y1 > y0 ? 1 : -1);
				D -= 2 * dx;
			}
			D += 2 * dy;
		}
	else for (int x = x0; x <= x1; x += 1)
	{
			image.set(x, y, color);
		if (D > 0) {
			y += (y1 > y0?1:-1);
			D -= 2 * dx;
		}
		D +=  2*dy; 
	}

#ifndef __DEBUG
	std::cerr << std::endl;
#endif
}

int main(int argc, char** argv) 
{

	TGAImage image(width, height, TGAImage::RGB);
	if (2 == argc)
	{
		model = new Model(argv[1]);
	}
	else
	{
		model = new Model("obj/sphere.obj");
	}
	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> face = model->face(i);
		for  (int j = 0; j < 3;  j++)
		{
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j+1)%3]);
			int x0 = (v0.x + 1)*width/2;
			int y0 = (v0.y + 1)*height/2; 
			int x1 = (v1.x + 1)*width /2;
			int y1 = (v1.y + 1)*height/2;
			line(x0, y0, x1, y1, image, white);
		}
	}
	image.flip_vertically();
	image.write_tga_file("Debug/12032020wireframe.tga");
	return 0;
}

