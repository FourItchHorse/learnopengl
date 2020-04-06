#include <GL/glut.h>
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1.0, 0.0, 0.0);
	 	glVertex2f(-0.5, -0.3);
		glVertex2f(0.5,  -0.3);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(-0.5, -0.0);
		glVertex2f(0.5, -0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2f(-0.5, 0.3);
		glVertex2f(0.5, 0.3);
	glEnd();
	glFlush();
}
void init() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_LINE_STIPPLE);
	glLineWidth(2.0);
	glLineStipple(3, 0xcccc);

	
	glEnable(GL_POLYGON_STIPPLE);
	const unsigned char x = (char) 0x0fff;
	glPolygonStipple(&x); 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, 1.0, -1.0);
}
int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
