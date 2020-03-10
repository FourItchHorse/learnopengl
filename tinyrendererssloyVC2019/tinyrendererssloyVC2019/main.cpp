#include "tgaimage.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
void line(int x0, int x1, int y0, int y1, TGAImage &image, TGAColor color) 
{
	bool steep = false;
	if (std::abs(y1 - y0) > std::abs(x1 - x0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	int D, dx, dy;
	dy = y1 - y0;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	if (x0 > x1)
		std::swap(x0, x1);
	dx = x1 - x0;
	D = (2 * dy) - dx; 
	int y = y0;
	
	for (int x = x0; x <= x1; x += 1)
	{
		if (steep)
			image.set(y, x, color);	
		else
			image.set(x, y, color);
		if (D > 0) {
			y += yi;
			D -= 2 * dx;
		}
		D +=  2*dy; 
	}
}

int main(int argc, char** argv) 
{

	TGAImage image(100, 100, TGAImage::RGB);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red); 
	line(13, 20, 80, 40, image, white);
	line(0, 100, 0, 100, image, white);
	image.flip_vertically();
	image.write_tga_file("Debug/10032020lines.tga");
	return 0;
}

