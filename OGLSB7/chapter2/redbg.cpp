#include <GL/gl3w.h>

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <string>
	void display(GLFWwindow* window) 
	{
					
		while(!glfwWindowShouldClose(window)) 
		{
         		double time = glfwGetTime();
			static const GLfloat BG[] = {(float) sin(time) * 0.5f +  0.5f, 
						     (float) cos(time) * 0.5f + 0.5f,
					        	0.0f, 1.0f};
			glClearBufferfv(GL_COLOR, 0, BG);
			glfwSwapBuffers(window);
		}
	}
	GLFWwindow* CreateWindowAndContext(GLuint width, GLuint height, const char* name) 
	{
		if(!glfwInit()) 
		{
			fprintf(stderr, "Failed to initialise GLFW\n");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!window) 
		{
			fprintf(stderr, "Failed to open window\n");
		}
		glfwMakeContextCurrent(window);
		 if(!gl3wInit()) 
		{
			fprintf(stderr, "Failed to initialise GL3W\n");
		} 
		fprintf(stderr, "VENDOR: %s\n", (char*)glGetString(GL_VENDOR));
		fprintf(stderr, "VERSION: %s\n", (char*)glGetString(GL_VERSION));
		fprintf(stderr, "RENDERER: %s\n", (char*)glGetString(GL_RENDERER));
		return window;
	}

	int main () 
	{
		GLFWwindow* myWindow = CreateWindowAndContext(800, 600, "04032020");
		display(myWindow);
		return 0;
	}	
		

