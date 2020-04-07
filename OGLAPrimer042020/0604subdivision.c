#include <GL/glut.h>
#include <math.h>
GLfloat v[3][2] = {{-0.0, -0.5},{-0.5, 0.5}, {0.5, 0.5}};
int n = 5;
float twist = 0.0f;
void triangle(GLfloat *a, GLfloat *b, GLfloat *c) 
{
	GLfloat v[2];
	double d;
	glBegin(GL_POLYGON);
		d = sqrt(a[0]*a[0] + a[1] * a[1]);
		v[0] = cos(twist*d)*a[0] - sin(twist*d)*a[1];
		v[1] = sin(twist*d)*a[0] + cos(twist*d)*a[1];
		glColor3f(1 - v[0] + twist, 1 - v[1] - twist, d);
		glVertex2fv(v);
		d = sqrt(b[0]*b[0] + b[1] * b[1]);
		v[0] = cos(twist*d)*b[0] - sin(twist*d)*b[1];
		v[1] = sin(twist*d)*b[0] + cos(twist*d)*b[1];
		glColor3f(1 - v[0] - twist, 1 - v[1] + twist, d);
		glVertex2fv(v);
		d = sqrt(c[0]*b[0] + b[1] * b[1]);
		v[0] = cos(twist*d)*c[0] - sin(twist*d)*c[1];
		v[1] = sin(twist*d)*c[0] + cos(twist*d)*c[1];
		glColor3f(1 - v[0], 1 - v[1], d + twist);
		glVertex2fv(v);
	glEnd();
}
void divide_triangle(GLfloat *a, GLfloat *b, GLfloat *c, int m)
{
	GLfloat v[3][2];
	int j;
	if(m > 0)
	{
		for(j = 0; j < 2; j++)v[0][j] = (a[j] + b[j])/2;
		for(j = 0; j < 2; j++)v[1][j] = (a[j] + c[j])/2;
		for(j = 0; j < 2; j++)v[2][j] = (b[j] + c[j])/2;
		divide_triangle(a, v[0], v[1], m-1);
		divide_triangle(v[0], b, v[2], m-1);
		divide_triangle(v[1], v[2], c, m-1);
		divide_triangle(v[0], v[1], v[2], m-1);
	}
	else (triangle(a, b, c));
	
}
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	divide_triangle(v[0], v[1], v[2], n);
	glFlush();
}
void init() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, 1.0, -1.0);
}
void processInput(unsigned char key, int x, int y) 
{
	float increment = 0.5;
	if (key == 'd')
		twist += increment;
	if( key == 'a')
		twist -= increment;
	if( key == 'w')
		n += 1;
	if( key == 's' && n > 0)
		n -= 1;
	glutPostRedisplay();
}
int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("simple");
	glutKeyboardFunc(processInput);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
