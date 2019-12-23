#include <GL/freeglut.h>
#include <stdio.h>
GLfloat v[3][2] = {0.0f, 0.5, -0.5f, -0.5f, 0.5f, -0.5f};
int n = 4;
void triangle(GLfloat *a, GLfloat *b, GLfloat *c) 
{
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
           glVertex2fv(a);
	glColor3f(0.0, 1.0, 0.0);
	   glVertex2fv(b);
	glColor3f(0.0, 0.0, 1.0);
	   glVertex2fv(c);
	glEnd();
	printf("TRIANGLE ERROR: %u\n", glGetError());
}
void divideTriangle(GLfloat *a, GLfloat *b, GLfloat *c, int m)
{
	GLfloat v[3][2];
	int j;
	if(m>0) 
	{
		for(j = 0; j < 2; j++) v[0][j] = (a[j] + b[j])/2;
		for(j = 0; j < 2; j++) v[1][j] = (a[j] + c[j])/2;
		for(j = 0; j < 2; j++) v[2][j] = (b[j] + c[j])/2;
		divideTriangle(a, v[0], v[1], m -1);
		divideTriangle(v[0], b, v[2], m -1);
		divideTriangle(v[1], v[2], c, m-1);
		divideTriangle(v[0], v[1], v[2], m-1);
	}
	else(triangle(a,b,c));
	printf("SUBDIVISION ERROR: %u\n", glGetError());
}
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	divideTriangle(v[0], v[1], v[2], n);
	glFlush();
	printf("DISPLAY ERROR: %u\n", glGetError());

}
void init() 
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glColor3f(0.75, 0.75, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	printf("INIT ERROR: %u\n", glGetError());
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("subdivision");
	printf("WINDOW INIT ERROR: %u\n", glGetError());
	glPolygonMode(GL_FRONT, GL_LINE);
	glutDisplayFunc(display);
	init();
	glutMainLoop();

}
