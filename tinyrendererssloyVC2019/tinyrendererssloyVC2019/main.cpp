#include "tgaimage.h"
#include <iostream>

TGAColor white = TGAColor(255, 255, 255, 255);
void line(int x0, int x1, int y0, int y1, TGAImage &image, TGAColor color) 
{
	float D, dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	D = (2 * dy) - dx; //D = 2A - B 
	int y = y0;
	for (int x = x0; x <= x1; x+= 1)
	{
		//image.set(x, y, color);
		std::cout << "Line coordinates: (" << x << "," << y << ")" << std::endl;
		dx = x1 - x;
		dy = y1 - y;
		D = (2 * dy) - dx;
		if (D > 0)
		{
			y += 1;
			/*D -= dx; 
			D /= 2; */ //D = A
		}
		//D += dx; //D = A + B
	}
}

int main(int argc, char** argv) 
{
	TGAImage image;
	line(0, 9, 0, 4, image, white);
	image.write_tga_file("10032020.tga");
	return 0;
}