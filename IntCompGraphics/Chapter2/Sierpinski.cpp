#include <GL/glut.h>
#include <stdlib.h>


void init() 
{
	const int numPoints = 5000;
	GLfloat points[numPoints][2];

	GLfloat vertices[3][2]={{-1.0, -1.0}, {0.0, 1.0},{1.0, -1.0}};

	points[0][0] = 0.25;
	points[0][1] = 0.5;
	for(int k = 1; k < numPoints; k++)
	{
		int j = rand() % 3;

		for(int i = 0; i < 2; i++) 
		points[k][i] = (points[k - 1][i] + vertices[j][i])/2.0;
	}
}
int main () {
      return 0;
}
