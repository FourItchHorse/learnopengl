#include <GL/freeglut.h>
void display () 
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_LINE_LOOP);	
          glVertex2f(0.0, 0.75);
	  glVertex2f(-0.5, -0.75);
	  glVertex2f(0.75, 0.15);
	  glVertex2f(-0.75, 0.15);
	  glVertex2f(0.5, -0.75);
	glEnd();


	glFlush();	
}		

void init() 
	
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glColor3f(0.75, 0.75, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	init();
	glutMainLoop();

}
