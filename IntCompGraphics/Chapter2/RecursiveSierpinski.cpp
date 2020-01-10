#include <GL/glew.h>
#include <GL/freeglut.h>
#include "InitShader.cpp"
const int nDivision = 4;
const int nTri = 243;
const int nVert = 729;

float points[nVert][5];
int index = 0;
void triangle(float* a, float* b, float* c) 
{
	for(int j = 0; j < 2; j++) {
	points[index++][j] = a[j];
	points[index++][j] = b[j];
	points[index++][j] = c[j];
	}
}
void divide_triangle(float* a, float* b, float* c, int k) 
{
	float tri[3][2];
	if(k > 0) 
	{
		for(int j = 0; j < 2; j++) {
		tri[0][j] = (a[j] + b[j])/2.0;
		tri[1][j] = (a[j] + c[j])/2.0;
		tri[2][j] = (b[j] + c[j])/2.0;
		}
		divide_triangle(a, tri[0], tri[1], k-1);
		divide_triangle(c, tri[1], tri[2], k-1);
		divide_triangle(b, tri[2], tri[0], k-1);
	} else  { triangle(a,b,c); } 
}

void init() {

	GLfloat vertices[3][2]={{-1.0, -1.0}, {0.0, 1.0},{1.0, -1.0}};

	divide_triangle(vertices[0], vertices[1], vertices[2], nDivision);
	
	GLuint program = ShaderReader::InitShader("SierpinskiVert.glsl", 
			"SierpinskiRecursiveFrag.glsl");
	glUseProgram(program); 

	GLuint vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, 
			GL_STATIC_DRAW); 

	
	GLuint loc = glGetAttribLocation(program, "pos");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 
			5*sizeof(float),(void*)0); 
	GLuint col= glGetAttribLocation(program, "col");
	glEnableVertexAttribArray(col);
	glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 
			5*sizeof(float),(void*)0); 


	glClearColor(0.0, 0.0, 0.0, 1.0);

}
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, nTri);
	glFlush();
}

int main (int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Non-interactive Sierpinski gasket");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
