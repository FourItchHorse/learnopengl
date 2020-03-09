#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line (int x0, int x1,  int y0, int y1, TGAImage &image, TGAColor color) 
{
	if (x0 > x1)
	{
		int tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (y0 > y1)
	{
		int tmp = y0;
		y0 = y1;
		y1 = tmp;
	}
	
	for (int x= x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1; y++)
		{
			image.set(x,y,color);
		}
		
	}
	
	
	
}

int main (int argc, char** argv)
{
	TGAImage image(100, 100, TGAImage::RGB);
	image.flip_vertically();
	line(13, 20, 80, 40, image, white);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red);
	image.write_tga_file("output.tga");
	return 0;
}	

