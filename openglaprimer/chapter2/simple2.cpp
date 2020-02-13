#include <GL/glut.h>
void triangle(GLfloat *a, GLfloat *b, GLfloat *c) 
{
	glBegin(GL_TRIANGLES);
	glVertex2fv(a);
	glVertex2fv(b);
	glVertex2fv(c);
	glEnd();

}
void display() 
{
   glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	triangle({0.0, 0.0}, {0.5, 1.0}, {1.0, 1.0});
   glFlush();
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
