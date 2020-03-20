#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
GLuint program, vao;
static const GLchar* vertexShaderSource = R"glsl(
#version 450 core
layout (location = 0) in vec4 offset;
layout (location = 1) in vec4 color;
out VS_OUT
{
	vec4 color;
} vs_out;
void main(void) 
{
	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
				         vec4(-0.25, -0.25, 0.5, 1.0),
					 vec4(0.25, 0.25, 0.5, 1.0));
	gl_Position = vertices[gl_VertexID] + offset;
	vs_out.color = color;
}
)glsl";
static const GLchar* tessControlShaderSrc = R"glsl(
#version 450 core
layout (vertices = 3) out;
void main(void)
{
if(gl_InvocationID == 0)
{
	gl_TessLevelInner[0] = 5.0;
	gl_TessLevelOuter[0] = 5.0;
 	gl_TessLevelOuter[1] = 5.0;
	gl_TessLevelOuter[2] = 5.0;
}
gl_out[gl_InvocationID].gl_Position =
	gl_in[gl_InvocationID].gl_Position;
}
)glsl";
static const GLchar* tessEvalShaderSrc= R"glsl(
#version 450 core
layout (triangles, equal_spacing, cw) in;
void main (void)
{
gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +
	           gl_TessCoord.y * gl_in[1].gl_Position +
	           gl_TessCoord.z * gl_in[2].gl_Position);
}
)glsl";
static const GLchar* fragmentShaderSource = R"glsl(
#version 450 core
in VS_OUT
{
	vec4 color;
}fs_in;
out vec4 color;
void main(void) 
{
	color = fs_in.color;
}
)glsl";
void compileShader(const char* source, GLenum type, GLuint program) 
{
	GLuint myShader = glCreateShader(type);
	glShaderSource(myShader, 1, &source, NULL);
	glCompileShader(myShader);
	GLint compiled;
	GLchar log[1024];
	glGetShaderiv(myShader, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
	glGetShaderInfoLog(myShader, 1024, NULL, log);
	fprintf(stderr,"\n%s\n", log);
	}
	glAttachShader(program, myShader);
	glDeleteShader(myShader);

}
void linkProgram(GLuint program) 
{
	glLinkProgram(program);
	int linked;
	GLchar log[1024];
	glGetShaderiv(program, GL_LINK_STATUS, &linked);
	if(!linked) {
	glGetProgramInfoLog(program, 1024, NULL, log);
	fprintf(stderr,"\n%s\n", log);
	}	
}
void init() {
	program = glCreateProgram();
        compileShader(vertexShaderSource, GL_VERTEX_SHADER, program);    		
        compileShader(tessControlShaderSrc, GL_TESS_CONTROL_SHADER, program);
		compileShader(tessEvalShaderSrc, GL_TESS_EVALUATION_SHADER, program);  		
        compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, program);
	linkProgram(program);	
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void shutdown() {
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
}
	
void display() {
	float currentTime = (float) glfwGetTime();
	float redY = sin(currentTime) * 0.5f + 0.5f;
	float greenX = cos(currentTime) * 0.5f + 0.5f;
	const GLfloat color[] = {redY, greenX, 0.0, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);
	GLfloat attrib[] = {sin(currentTime) * 0.5f,
			    cos(currentTime) * 0.6f,
			    0.0f, 0.0f};
	glVertexAttrib4fv(0, attrib);
	GLfloat orange[] = {1 - redY, 1 - greenX, 0.5, 1.0f};
	glVertexAttrib4fv(1, orange);
	glUseProgram(program);
	glDrawArrays(GL_PATCHES, 0, 3);
}


int main(int argc, char** argv) {
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialise GLFW!\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow * window = glfwCreateWindow(600, 400, "Render BG", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if(gl3wInit()) 
	{
		fprintf(stderr, "Failed to create opengl context\n");
		return -1;
	}
	fprintf(stderr, "VENDOR: %s\n", (char*) glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char*) glGetString(GL_VERSION));	
	fprintf(stderr, "RENDERER: %s\n", (char*) glGetString(GL_RENDERER));

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
