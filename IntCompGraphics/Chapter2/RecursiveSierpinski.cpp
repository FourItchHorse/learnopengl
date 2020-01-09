#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include "InitShader.cpp"
const int nDivision = 4;
const int count = static_cast<int>(pow(3, static_cast<double>(nDivision)));
void init() {}
	GLfloat points[numPoints][5];

	GLfloat vertices[3][2]={{-1.0, -1.0}, {0.0, 1.0},{1.0, -1.0}};


	
	GLuint program = ShaderReader::InitShader("SierpinskiVert.glsl", 
			"SierpinskiFrag.glsl");
	glUseProgram(program); 

	GLuint vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); 

	
	GLuint loc = glGetAttribLocation(program, "pos");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 
			5*sizeof(float),(void*)0); 

	GLuint col = glGetAttribLocation(program, "col");
	glEnableVertexAttribArray(col);
	glVertexAttribPointer(col, 2, GL_FLOAT, GL_FALSE, 
			5*sizeof(float),(void*)(2*sizeof(float))); 
}
void triangle(float *a, float *b, float *c) 
{
	static int i = 0;
        
	points[i] = a;
	i++;
	points[i] = b;
	i++;
	points[i] = c;
	i++;
}
void divide_triangle(float *a, float *b, float *c, int k) 
{
	float tri[3][2];
	if(k > 0) 
	{
		for(int j = 0; j < 2; j++) {
		tri[0][j] = (a[j] + b[j])/2.0;
		tri[1][j] = (a[j] + c[j])/2.0;
		tri[2][j] = (b[j] + c[j])/2.0;
		}
	} else  { triangle(a,b,c); } 
}

void display() 
{
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
