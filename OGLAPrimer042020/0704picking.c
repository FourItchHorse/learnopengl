#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
void drawObjects(GLenum mode)
{
    if(mode == GL_SELECT)glLoadName(1);
    glColor3f(1.0, 0.0, 0.0);
    glRectf(-0.5, 0.5, 1.0, 1.0);
    if(mode == GL_SELECT)glLoadName(2);
    glColor3f(0.0, 0.0, 1.0);
    glRectf(-1.0, -1.0, 0.5, 0.5);
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawObjects(GL_RENDER);
    glFlush();
}
void reshape(int w, int h) 
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, 2.0, -2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void init () 
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
}
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr;

	printf("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	
	for(i = 0; i < hits; i++)
	{
		names = *ptr;
		ptr += 3;
		for(j = 0;j<names;j++)
		{
		if(*ptr == 1)printf("red rectangle\n");
		else printf("blue rectangle\n");
		ptr++;
		}
	}
}
#define size 500
#define N 3
void mouse(int button, int state, int x, int y) 
{
    GLuint nameBuffer[size];
    GLint hits;
    GLint viewport[4];
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    	glInitNames();
	glPushName(0);
	glSelectBuffer(size, nameBuffer);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix((GLdouble) x, (GLdouble) y, N, N, viewport);

	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);

	glRenderMode(GL_SELECT);
	drawObjects(GL_SELECT);
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	hits = glRenderMode(GL_RENDER);

	processHits(hits, nameBuffer);

	glutPostRedisplay();

    }
}
int main (int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
}
