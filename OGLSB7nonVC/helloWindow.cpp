#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
void init() 
{

}
void shutdown() 
{
}
void display() 
{
	float time = glfwGetTime();
	glClearColor(sin(time) * 0.5 + 0.5, cos(time) * 0.5 + 0.5, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}
int main(int argc, char** argv) 
{
	if(!glfwInit()) {
		fprintf(stderr, "Unable to init glfw\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window!", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	int NoGLContext = gl3wInit();
	if(NoGLContext)
	{
		fprintf(stderr, "Unable to initialise opengl! %d\n", NoGLContext);
		return -1;
	}	

	init();
	while(!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
		display();
		glfwSwapBuffers(window);
	}
	shutdown();
	return 0;

}
