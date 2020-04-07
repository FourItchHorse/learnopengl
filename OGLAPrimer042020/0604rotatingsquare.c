#include <GL/glut.h>
#include <math.h>
#define DEG_TO_RAD 0.017453
GLfloat theta = 0.0;
void myidle()
{
	theta += 0.005;
	if(theta > 360.0) theta -= 360.0;
	glutPostRedisplay();
}
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
		glVertex2f(sin(DEG_TO_RAD*theta), cos(DEG_TO_RAD*theta));
		glVertex2f(-cos(DEG_TO_RAD*theta), sin(DEG_TO_RAD*theta));
		glVertex2f(-sin(DEG_TO_RAD*theta), -cos(DEG_TO_RAD*theta));
		glVertex2f(cos(DEG_TO_RAD*theta), -sin(DEG_TO_RAD*theta));
	glEnd();
	glFlush();
}
void init() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

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
	glutIdleFunc(myidle);
	init();
	glutMainLoop();
}
