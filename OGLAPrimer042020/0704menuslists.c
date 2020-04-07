#include <GL/glut.h>

#define GREEN_TRI 1
char rendering;
void display() 
{
	if(rendering == 1) {
		glClear(GL_COLOR_BUFFER_BIT);
		glCallList(GREEN_TRI);
		glutSwapBuffers();
	}
	else if (rendering == 0) {
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers();
	}
}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, 1.0, -1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void render_menu(int id)
{
  if(id == 1)rendering = 1;
  if(id == 2)rendering = 0;
  glutPostRedisplay();
}
int main (int argc, char** argv)
{
	glNewList(GREEN_TRI, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(0.0, 0.5, 0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.0, -0.5);
		glVertex2f(0.5, 0.5);
		glPopAttrib();
	glEndList();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Display lists and menus");
	glutDisplayFunc(display);
	init();

	glutCreateMenu(render_menu);
	glutAddMenuEntry("Render tri", 1);
	glutAddMenuEntry("Render clear", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}
