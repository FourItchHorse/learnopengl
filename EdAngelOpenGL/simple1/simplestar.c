#include <GL/glut.h>

void display () 
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_POLYGON);	
          glVertex2f(0.0, 0.75);
	  glVertex2f(-0.25, 0.25);
	  glVertex2f(0.25, 0.25);
	  glVertex2f(0.5, -0.5);
	  glVertex2f(-0.5, 0.25);
	  glVertex2f(0.5, 0.25);
	glEnd();

	glFlush();	
}		

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	glutMainLoop();

}	
