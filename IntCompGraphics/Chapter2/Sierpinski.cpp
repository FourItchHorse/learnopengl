#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>

#include "InitShader.cpp"
	const int numPoints = 5000;
void init(void) 
{
	GLfloat points[numPoints][5];

	GLfloat vertices[3][2]={{-1.0, -1.0}, {0.0, 1.0},{1.0, -1.0}};

	points[0][0] = 0.25;
	points[0][1] = 0.5;
	points[0][2] = 1.0;
	points[0][3] = 1.0;
	points[0][4] = 1.0;
	for(int k = 1; k < numPoints; k++)
	{
		int j = rand() % 3;

		for(int i = 0; i < 2; i++) 
		points[k][i] = (points[k - 1][i] + vertices[j][i])/2.0;
		
		points[k][j + 2] = 1.0;
		points[k][4] = 1.0f;
		
	}

	
	GLuint program = ShaderReader::InitShader("SierpinskiVert.glsl", "SierpinskiFrag.glsl");
	glUseProgram(program); 

	GLuint abuffer;

	glGenVertexArrays(1, &abuffer);
	glBindVertexArray(abuffer);

	GLuint buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); 

	
	GLuint loc = glGetAttribLocation(program, "pos");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void*)0); 

	GLuint col = glGetAttribLocation(program, "col");
	glEnableVertexAttribArray(col);
	glVertexAttribPointer(col, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void*)(2*sizeof(float)));
	glClearColor(1.0, 1.0, 1.0, 1.0); 
}
void mydisplay() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_POINTS, 0, numPoints);
	glFlush();
}
int main (int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Non-interactive Sierpinski gasket");
	glewInit();
	init();
	glutDisplayFunc(mydisplay);
	glutMainLoop();
	return 0;
}
