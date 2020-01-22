#include <GL/glut.h>
       GLfloat v[3][2] = {0.0, 0.75, -0.75, -0.75, 0.75, -0.75};
       int n = 2;
void triangle(GLfloat *a, GLfloat *b, GLfloat *c) 
{
	glBegin(GL_TRIANGLES);
	glVertex2fv(a);
	glVertex2fv(b);
	glVertex2fv(c);
	glEnd();

}
void divide_triangle (GLfloat *a, GLfloat *b, GLfloat *c, int m) 
{
	GLfloat v[3][2];
	int j;
	if(m>0) 
	{
		for(j = 0; j < 2; j++) v[0][j] = (a[j] + b[j])/2;
		for(j = 0; j < 2; j++) v[1][j] = (a[j] + c[j])/2;
		for(j = 0; j < 2; j++) v[2][j] = (b[j] + c[j])/2;
		divide_triangle(a, v[0], v[1], m - 1);
		divide_triangle(v[0], b, v[2], m - 1);
		divide_triangle(v[1], v[2], c, m - 1);
		divide_triangle(v[0], v[1], v[2], m - 1);
	}
	else 
	{
		triangle(a, b, c);
	}
}
void display() 
{
   glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	divide_triangle(v[0], v[1], v[2], n);
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
