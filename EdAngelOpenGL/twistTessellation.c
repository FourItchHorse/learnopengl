#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
GLfloat v[3][2] = {0.0f, 0.5, -0.5f, -0.5f, 0.5f, -0.5f};
int n = 4;
float twist = 3;
void triangle(GLfloat *a, GLfloat *b, GLfloat *c, int type) 
{
	glPolygonMode(GL_FRONT, GL_LINE);
	GLfloat v[2];
	double d;
	glBegin(GL_TRIANGLES);
	switch(type) 
	{
		case(2):
		case(4):
		case(6):
		case(7):
			glEdgeFlag(GL_FALSE);
		break;
		default:
			glEdgeFlag(GL_TRUE);
	}
	d = sqrt(a[0]*a[0] + a[1]*a[1]);
	v[0] = cos(twist*d)*a[0]-sin(twist*d)*a[1];
	v[1] = cos(twist*d)*a[0]+cos(twist*d)*a[1];
           glVertex2fv(v);
	   switch(type) 
	{
		case(3):
		case(4):
		case(5):
		case(7):
			glEdgeFlag(GL_FALSE);
		break;
		default:
			glEdgeFlag(GL_TRUE);
	}
	d = sqrt(b[0]*b[0] + b[1]*b[1]);
	v[0] = cos(twist*d)*b[0]-sin(twist*d)*b[1];
	v[1] = cos(twist*d)*b[0]+cos(twist*d)*b[1];
           glVertex2fv(v);
	switch(type)
	{
		case(1):
		case(5):
		case(6):
		case(7):
			glEdgeFlag(GL_FALSE);
		break;
		default:
			glEdgeFlag(GL_TRUE);
	}
	d = sqrt(c[0]*c[0] + c[1]*c[1]);
	v[0] = cos(twist*d)*c[0]-sin(twist*d)*c[1];
	v[1] = cos(twist*d)*c[0]+cos(twist*d)*c[1];
           glVertex2fv(v);
	 
	glEnd();
	printf("TRIANGLE ERROR: %u\n", glGetError());
}
void divideTriangle(GLfloat *a, GLfloat *b, GLfloat *c, int m, int k)
{
	GLfloat v[3][2];
	int j, flag[4];
	if(m>0) 
	{
		for(j = 0; j < 2; j++) {
		v[0][j] = (a[j] + b[j])/2;
		v[1][j] = (a[j] + c[j])/2;
		v[2][j] = (b[j] + c[j])/2;
		}
		switch(k) 
		{
			case(0):
				flag[0] = 3;
				flag[1] = 1;
				flag[2] = 2;
			break;
			case(1):
				flag[0] = 5;
				flag[1] = 1;
				flag[2] = 6;
			break;
			case(2):
				flag[0] = 4;
				flag[1] = 6;
				flag[2] = 2;
			break;
			case(3):
				flag[0] = 3;
				flag[1] = 5;
				flag[2] = 4;
			break;
			case(4):
				flag[0] = 4;
				flag[1] = 7;
				flag[2] = 4;
			break;
			case(5):
				flag[0] = 5;
				flag[1] = 5;
				flag[2] = 7;
			break;
			case(6):
				flag[0] = 7;
				flag[1] = 6;
				flag[2] = 6;
			break;
			case(7):
				flag[0] = 7;
				flag[1] = 7;
				flag[2] = 7;
			break;
		}
		flag[3] =7;
		divideTriangle(a, v[0], v[1], m -1, flag[0]);
		divideTriangle(v[0], b, v[2], m -1, flag[1]);
		divideTriangle(v[1], v[2], c, m-1, flag[2]);
		divideTriangle(v[0], v[1], v[2], m-1, flag[3]);
	}
	else(triangle(a,b,c, k));
	printf("SUBDIVISION ERROR: %u\n", glGetError());
}
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	triangle(v[0], v[1], v[2],/* n,*/ 0);
	glFlush();
	printf("DISPLAY ERROR: %u\n", glGetError());

}
void init() 
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glColor3f(0.75, 0.75, 0.0);

	glPolygonMode(GL_FRONT, GL_LINE);

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
	glutDisplayFunc(display);
	init();
	glPolygonMode(GL_FRONT, GL_LINE);
	glutMainLoop();

}
