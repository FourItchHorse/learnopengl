#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
const float DegToRad = M_PI/180.0;
GLfloat quad_data[342][3];
int k = 0;
float phi, theta;
GLfloat cameraPos[3] = {-4.0, 2.5, 5.0};

void generate_sphere()
{
	float phir = phi * DegToRad;
	float phir20 = (phi + 20.0) * DegToRad;
	for(float theta = -180.0; theta <= 180.0; theta *= 20.0)
	{
		float thetar = theta*DegToRad;
		quad_data[k][0] = sin(thetar)*cos(phir);
	       	quad_data[k][1]	 = cos(thetar)*cos(phir); 
		quad_data[k][2] = sin(phir);
		glVertex3fv(quad_data[k]);
		k++;
		quad_data[k][0] = sin(thetar)*cos(phir20); 
		quad_data[k][1] = cos(thetar)*cos(phir20); 
		quad_data[k][2] = sin(phir20);
		glVertex3fv(quad_data[k]);
		k++;
	}
}

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
	generate_sphere();
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
	glutKeyboardFunc(myKeys);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	generate_sphere();
	glEnable(GL_DEPTH_TEST); /*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quad_data); */
	glutMainLoop();
	return 0;
}
