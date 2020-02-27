#include <GL/glut.h>
#include <math.h>
#define DEG_TO_RAD 0.017453
GLfloat theta = 0.0;
void mydisplay() 
{
   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_POLYGON);
   	glVertex2f(-sin(DEG_TO_RAD * theta)), cos(DEG_TO_RAD * theta);
   	glVertex2f(cos(DEG_TO_RAD * theta)), sin(DEG_TO_RAD * theta);
   	glVertex2f(-cos(DEG_TO_RAD * theta)), -sin(DEG_TO_RAD * theta);
   	glVertex2f(sin(DEG_TO_RAD * theta)), -cos(DEG_TO_RAD * theta);
   glEnd();	
   glutSwapBuffers();
}
void myidle()
{
	theta += 0.125;
	if(theta > 360.0) theta -= 360.0;
       glutPostRedisplay();	
}
void init() 
{
	glClearColor(0.0, 0.0 ,0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( -1.0, 1.0, -1.0, 1.0);
}
int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("rotation");
	glutIdleFunc(myidle); 
	glutDisplayFunc(mydisplay);
	init();
	glutMainLoop();
}

