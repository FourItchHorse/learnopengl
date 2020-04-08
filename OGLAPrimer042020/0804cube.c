#include <GL/glut.h>
GLfloat vertices[][3] = {{-1.0, -1.0, 1.0},{-1.0, 1.0, 1.0},{1.0, 1.0, 1.0},{1.0, -1.0, 1.0},
		     {-1.0, -1.0, -1.0},{-1.0, 1.0, -1.0},{1.0, 1.0, -1.0},{1.0, -1.0, -1.0}};
GLfloat colors[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
		       {1.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}};
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	
}
void polygon(int a, int b, int c, int d) 
{
	glBegin(GL_POLYGON);
		glVertex3fv(vertices[a]);
		glVertex3fv(vertices[b]);
		glVertex3fv(vertices[c]);
		glVertex3fv(vertices[d]);
	glEnd();
}
void cube()
{
	glColor3fv(colors[0]);
	polygon(0, 3, 2, 1);
	glColor3fv(colors[1]);
	polygon(2, 3, 7, 6);
	glColor3fv(colors[2]);
	polygon(3, 0, 4, 7);
	glColor3fv(colors[3]);
	polygon(1, 2, 6, 5);
	glColor3fv(colors[4]);
	polygon(4, 5, 6, 7);
	glColor3fv(colors[5]);
	polygon(5, 4, 0, 1);
}
void display(){

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2.0, -2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	cube();
	glutSwapBuffers();
}
void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
}
int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}	
