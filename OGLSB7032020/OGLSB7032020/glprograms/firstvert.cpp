//file initialised 07/03/2020
#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>

const GLchar * vertexShader = R"glsl(

)glsl";
const GLchar* fragmentShader = R"glsl(

)glsl";


void display() 
{

}
void init() 
{

}
int main(int argc, char** argv) 
{
	fprintf(stderr, "Using main from 07032020 first vert");
	int major = 4;
	int minor = 3;
	if (!glfwInit())
	{
		fprintf(stderr, "UNABLE TO CREATE WINDOW\n");
		return -1;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_VERSION_MINOR, minor);
	
	GLFWwindow* myWindow = glfwCreateWindow(800, 600, "First vert 07032020", nullptr, nullptr);
	 
	while (!glfwWindowShouldClose(myWindow))
	{
		display();
		glfwSwapBuffers(myWindow);
	}
}
