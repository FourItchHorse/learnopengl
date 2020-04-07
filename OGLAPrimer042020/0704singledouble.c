#include <GL/glut.h>
#include <math.h>
int singleb, doubleb;
#define DEG_TO_RAD 0.017453
float theta;
void myReshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, 2.0, -2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void spinDisplay()
{
	theta += 0.025;
	if(theta > 360.0) theta -= 360.0;
	glutSetWindow(singleb);
	glutPostWindowRedisplay(singleb);
	glutSetWindow(doubleb);
	glutPostWindowRedisplay(doubleb);
}
void myInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, 1.0, -1.0);
}

void displays()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
		glVertex2f(sin(DEG_TO_RAD * theta), 
			   cos(DEG_TO_RAD * theta));
		glVertex2f(sin(DEG_TO_RAD * theta),
			    -cos(DEG_TO_RAD * theta));
		glVertex2f(-sin(DEG_TO_RAD * theta), 
			   -cos(DEG_TO_RAD * theta));
		glVertex2f(-sin(DEG_TO_RAD * theta), 
			    cos(DEG_TO_RAD * theta));
	glEnd();
	glFlush();
}
void displayd() 
{	
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
		glVertex2f(sin(DEG_TO_RAD * theta), 
			   cos(DEG_TO_RAD * theta));
		glVertex2f(sin(DEG_TO_RAD * theta),
			    -cos(DEG_TO_RAD * theta));
		glVertex2f(-sin(DEG_TO_RAD * theta), 
			   -cos(DEG_TO_RAD * theta));
		glVertex2f(-sin(DEG_TO_RAD * theta), 
			   cos(DEG_TO_RAD * theta));
	glEnd();
	glutSwapBuffers();
}
void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		glutIdleFunc(spinDisplay);
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		glutIdleFunc(NULL);
}
void mykey(unsigned char key, int x, int y)
{
	if(key == 'Q' || key == 'q')exit(0);
}
void quit_menu(int id)
{
	if(id == 1)exit(0);
}
int main (int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	singleb = glutCreateWindow("Single buffered");
	myInit();
	glutDisplayFunc(displays);
	glutReshapeFunc(myReshape);
	glutIdleFunc(spinDisplay);
	glutMouseFunc(mouse);
	glutKeyboardFunc(mykey);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(310, 0);
	doubleb = glutCreateWindow("double buffered");
	myInit();
	glutDisplayFunc(displayd);
	glutReshapeFunc(myReshape);
	glutIdleFunc(spinDisplay);
	glutMouseFunc(mouse);

	glutCreateMenu(quit_menu);
	glutAddMenuEntry("quit", 1);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	glutMainLoop();
}
