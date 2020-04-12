#include <GL/glut.h>
#include <GL/glew.h>
void init()
{

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);
	glewInit();
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}
