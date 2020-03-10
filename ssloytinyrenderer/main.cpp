#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line (int x0, int x1,  int y0, int y1, TGAImage &image, TGAColor color) 
{
	int dx, dy, D, y;
	dx = x1 - x0;
	dy = y1 - y0;
	y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (D > 0) 
		y += 1;
		
	        D = 2*dy - dx;
		
		image.set(x,y, color);
	}
	

}

int main (int argc, char** argv)
{
	TGAImage image(100, 100, TGAImage::RGB);
	//image.flip_vertically();
	line(10, 50, 50, 10, image, white);
	image.write_tga_file("output.tga");
	return 0;
}	

