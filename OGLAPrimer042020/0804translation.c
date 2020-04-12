#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
GLfloat vertices[][3] = {{-1.0, -1.0, 1.0},{-1.0, 1.0, 1.0},{1.0, 1.0, 1.0},{1.0, -1.0, 1.0},
		     {-1.0, -1.0, -1.0},{-1.0, 1.0, -1.0},{1.0, 1.0, -1.0},{1.0, -1.0, -1.0}};
GLfloat colors[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
		       {1.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}};
GLubyte cubeIndices[] = {0, 3, 2, 1, 2, 3, 7, 6, 3, 0, 4, 7, 1, 2, 6, 5, 4, 5, 6, 7, 5, 4, 0, 1};
GLfloat cameraPos[3] = {-4.0, 2.5, 5.0};
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-3.0, 3.0, -3.0, 3.0, 5.0, 100.0);	
}
void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0.0, -3.0);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
	glTranslatef(-2.0, 1.0, -4.0);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
	glTranslatef(-3.0, 2.0, -5.0);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
	glutSwapBuffers();
}
void myKeys(unsigned char key, int x, int y)
{
	float increment = 0.1;
	if(key == 'a')cameraPos[0] -= increment;
	if(key == 'd')cameraPos[0] += increment;
	if(key == 'w')cameraPos[1] -= increment;
	if(key == 's')cameraPos[1] += increment;
	printf("%f, %f, %f\r", cameraPos[0], cameraPos[1], cameraPos[2]);
	glutPostRedisplay();
}
int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myKeys);
	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glClearColor(1.0, 1.0, 1.0, 1.0);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutMainLoop();
}	
