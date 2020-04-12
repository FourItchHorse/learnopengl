#include <GL/glut.h>
#include <stdio.h>
GLuint segments = 4;
GLfloat height = 1;
GLfloat verts[segments * (height + 1)][3];
GLfloat cameraPos[3] = {-4.0, 2.5, 5.0};
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-3.0, 3.0, -3.0, 3.0, 5.0, 100.0);	
}
void sphereTip()
{
}
void sphereCentre()
{
	for(int i = 0; i < height * 2; i += 2)

}
void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
	glutSwapBuffers();
}
void myKeys(unsigned char key, int x, int y)
{
	float increment = 0.1;
	if(key == 'a')cameraPos[0] -= increment;
	if(key == 'd')cameraPos[0] += increment;
	if(key == 'w')cameraPos[1] -= increment;
	if(key == 's')cameraPos[1] += increment;
	if(key == 'q')cameraPos[2] -= increment;
	if(key == 'e')cameraPos[2] += increment;
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
	glutMainLoop();
}	
