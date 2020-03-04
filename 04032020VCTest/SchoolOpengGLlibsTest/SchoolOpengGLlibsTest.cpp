// SchoolOpengGLlibsTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shaders.h";

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ElementBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColour = 1 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;


void init(void)
{
	static const GLfloat vertices[NumVertices][5] =
	{
		{-0.9, -0.9, 1.0, 1.0, 1.0},
		{0.85, -0.9, 0.0, 1.0, 0.0},
		{-0.9, 0.85, 0.0, 0.0, 1.0},

		{0.9, -0.85, 0.0, 0.0, 1.0},
		{0.9, 0.9,   0.0, 1.0, 0.0},
		{-0.85, 0.9, 1.0, 0.0, 0.0}
	};


	glCreateVertexArrays(NumVAOs, VAOs);

	glCreateBuffers(NumBuffers, Buffers);
	glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "27022020.vert" },
		{GL_FRAGMENT_SHADER, "27022020.frag" },
		{GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	glBindVertexArray(VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(vPosition);

	glVertexAttribPointer(vColour, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)2);
	glEnableVertexAttribArray(vColour);

}
void display(void)
{
	static const float bg[] = { 0.0, 0.0, 0.0, 0.0 };
	glClearBufferfv(GL_COLOR, 0, bg);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int main(int argc, char** argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Triangles", NULL, NULL);
	glfwMakeContextCurrent(window);
	gl3wInit();

	init();

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
