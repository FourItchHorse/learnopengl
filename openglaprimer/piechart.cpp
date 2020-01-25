#include <GL/glut.h>
#include <math.h>
void circle(GLfloat radius, int a, int b, int c, int d) 
{
	glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0, 0.0);	
	float x, y;
	double twopi = 2.0f * M_PI;
	int j = 0;
	for (float i = 0; i <= twopi ; i += (M_1_PI / 5.0f)) 
	{
		x = cos(i) * radius;
		y = sin(i) * radius;
			if(j == d)
			glColor3f(1.0, 0.0, 0.0);
			if (j == c) 
			glColor3f(0.0, 1.0, 0.0);
			if (j == b)
			glColor3f(0.0, 0.0, 1.0);
			if (j <= a) 
			glColor3f(1.0, 1.0, 0.0);
		glVertex2f(x, y);
		j++;
	}
	glEnd();

}
void display() 
{
   glClear(GL_COLOR_BUFFER_BIT);
   circle(0.75f, 5, 10, 25, 60);
   glFlush();
}
void init() 
{
	glClearColor(0.0, 0.0 ,0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glPolygonMode(GL_FRONT, GL_LINE);
	gluOrtho2D( -1.0, 1.0, -1.0, 1.0);
}
int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("piechart");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
