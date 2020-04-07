#include <GL/glut.h>
#include <stdio.h>
GLint myX, myY, xx, yy;
int ww, hh;
int points[100][2];
int i = 0;

void mymotion(int x, int y)
{
	do {
	points[i][0] = x;
	points[i][1] = y;
	i++;
	printf("%i, %i, %i,\r", x, y, i);
	}while(i < 100);
	i -= 100;
	glutPostRedisplay();
}
void entry(int state)
{
	if(state == GLUT_LEFT) glutPostRedisplay();
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
	glBegin(GL_LINE_LOOP);
	for(int j = 0; j < 100; j++)
	glVertex2i(points[j][0], points[j][1]);	
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
	glutEntryFunc(entry);
	glutMotionFunc(mymotion);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
