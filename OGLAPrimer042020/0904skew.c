#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
GLfloat vertices[][3] = {{-1.0, -1.0, 1.0},{-1.0, 1.0, 1.0},{1.0, 1.0, 1.0},{1.0, -1.0, 1.0},
		     {-1.0, -1.0, -1.0},{-1.0, 1.0, -1.0},{1.0, 1.0, -1.0},{1.0, -1.0, -1.0}};
GLfloat colors[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
		       {1.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}};
GLubyte cubeIndices[] = {0, 3, 2, 1, 2, 3, 7, 6, 3, 0, 4, 7, 1, 2, 6, 5, 4, 5, 6, 7, 5, 4, 0, 1};
GLfloat cameraPos[3] = {-4.0, 2.5, 5.0};
float theta;
float m[16];
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
	m[4] = 1.0/tan(theta);
	glLoadMatrixf(m);
	glMultMatrixf(m);
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
	if(key == 'z')theta += 0.05;
	if(key == 'x')theta -= 0.05;
	printf("%f, %f, %f, %f\r", cameraPos[0], cameraPos[1], cameraPos[2], theta);
	if(theta> 2 * M_PI)theta -= 2 * M_PI;
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
	for(int i = 0; i < 16; i++)m[i] = 0.0;
	for(int j = 5; j < 16;j+= 5)m[j] = 1.0;
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glClearColor(1.0, 1.0, 1.0, 1.0);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutMainLoop();
}	
