
#include <iostream>
#include <iostream>
using namespace std;

#include "../MyIncl/vgl.h"
#include "../MyIncl/LoadShaders.cpp"
enum VAO_IDs {Cube, NumVAOs };
enum Buffer_IDs {ArrayBuffer, NumBuffers };
enum Attrib_IDs {vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init(void) 
{
	static const GLfloat vertices[NumVertices][3] 
	{
		{-0.75, 0.75, -1.0},
		{-0.75, -0.75, -1.0},
		{0.75, 0.75, -1.0},
		{0.75, -0.75, -1.0},
		{0.85, -0.85, 1.0},
		{-0.85, 0.85, 1.0},
	};
	glCreateVertexArrays(NumVAOs, VAOs);
	glCreateBuffers(NumBuffers, Buffers);
	glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices),
			vertices, 0);
	ShaderInfo shaders[] {
		{GL_VERTEX_SHADER, "triangles.vert"},
		{GL_FRAGMENT_SHADER, "triangles.frag"},
		{GL_NONE, NULL}
	};

	GLuint program = ShaderReader::LoadShaders(shaders);
	glUseProgram(program);

	glBindVertexArray(VAOs[Cube]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 
			0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
}
void display(void) 
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
	glClearBufferfv(GL_COLOR, 0, black);

	glBindVertexArray(VAOs[Cube]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, NumVertices);
}
int main (int argc, char** argv) 
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "Cube", 
			NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	
	init();

	while(!glfwWindowShouldClose(window)) 
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
