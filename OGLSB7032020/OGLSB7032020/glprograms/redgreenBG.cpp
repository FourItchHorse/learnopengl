#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1 
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <string>

int huechange06032020();

/*int main(int argc, char* argv)
{
	fprintf(stderr, "Using main from 06032020 BG file");
	huechange06032020();
	return 0;
} */

void processInput(GLFWwindow* window) 
{
	
}

void display()
{
	double time = glfwGetTime();
	const GLfloat BG[] = { (float)sin(time) * 0.5f + 0.5f,
					 (float)cos(time) * 0.5f + 0.5f,
					 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, BG);

}


int huechange06032020()
{
	int majVer = 4;
	int minVer = 6;
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialise GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minVer);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "04032020", nullptr, nullptr);
	if (!window)
	{
		fprintf(stderr, "Failed to open window\n");
		return -1;
	}
	glfwMakeContextCurrent(window);
	

	if (gl3wInit())
	{
		fprintf(stderr, "Failed to initialise opengl\n");
		return gl3wInit();
	}
	

	fprintf(stderr, "VENDOR: %s\n", (char*)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char*)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char*)glGetString(GL_RENDERER));

	

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		display();
		glfwSwapBuffers(window);
	}
	return 0;
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
