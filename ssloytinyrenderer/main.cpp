#include "tgaimage.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
void line(int x0, int x1, int y0, int y1, TGAImage &image, TGAColor color) 
{
	if(x0 > x1)
		std::swap(x0,x1);
	if(y0 > y1)
		std::swap(y0, y1);
	float D, dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	D = (2 * dy) - dx; //D = 2A - B 
	int y = y0;
	for (int x = x0; x <= x1; x+= 1)
	{
		image.set(x, y, color);
		//std::cout << "Line coordinates: (" << x << "," << y << ")" << std::endl;
		dx = x1 - x;
		dy = y1 - y;
		D = (2 * dy) - dx;
		if (D > 0)
		{
			y += 1;
		}
	}
}

int main(int argc, char** argv) 
{
	TGAImage image(100, 100, TGAImage::RGB);
	line(13, 20, 80, 40, image, white);
	line(20, 13, 80, 40, image, red);
	line(80, 40, 13, 20, image, red);
	line(0, 50, 0, 25, image, white);
	image.write_tga_file("10032020lines.tga");
	return 0;
}
