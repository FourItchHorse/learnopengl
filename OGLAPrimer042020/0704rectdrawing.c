#include <GL/glut.h>
#include <stdio.h>
GLint myX, myY, xx, yy;
int ww, hh;
void mousecallback(int button, int state, int x, int y)
{
	static bool first = true;
	if(state == GLUT_DOWN&button == GLUT_LEFT_BUTTON)
	{
		printf("%i, %i\r", x, y);
		if(first)
		{
			myX = x;
			myY = hh - y;
			first = !first;
		}
		else 
		{
			first = !first;
			xx = x;
			yy = hh -y;	
		}
		glutPostRedisplay();
	}
}

void myReshape(GLsizei w, GLsizei h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) w, 0.0, (GLfloat) h);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,w,h);
	ww = w;
	hh = h;
}

void display() 
{			
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
		glVertex2i(myX,  myY);
		glVertex2i(myX, yy);
		glVertex2i(xx, yy);
		glVertex2i(xx, myY);
	glEnd();
	glFlush();
}
void init() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);

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
	glutReshapeFunc(myReshape);
	glutMouseFunc(mousecallback);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
