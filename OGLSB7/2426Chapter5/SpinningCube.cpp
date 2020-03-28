#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include <vmath/vmath.h>
#include <stdio.h>
#define _USE_MATH_DEFINES 1
#include <math.h>
GLuint program, vao, vbo;
mat4 mv_matrix, proj_matrix;
static const GLchar* vShaderSrc = R"glsl(
#version 450 core
in vec4 position;
out VS_OUT
{
    vec4 color;
} vs_out;
 uniform mat4 mv_matrix;
 uniform mat4 proj_matrix;
void main(void)
{
    gl_Position = proj_matrix * mv_matrix * position;
    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.4, 0.0);    
}
)glsl";
static const GLchar* fShaderSrc = R"glsl(
#version 450 core
out vec4 color;
in VS_OUT
{
    vec4 color
}fs_in;
void main(void)
{
    color = fs_in.color
}
)glsl";
void compileShader(const char* source, GLenum type, GLuint program) 
{
    GLuint myShader = glCreateShader(type);
    glShaderSource(myShader, 1, &source, NULL);
    glCompileShader(myShader);
    delete source;
    GLint compiled;
    glGetShaderiv(myShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) 
    {
        GLchar log[1024];
        glGetShaderInfoLog(myShader, 1024, NULL, log);
        fprintf(stderr, "\n%s\n", log);
        delete[] log;
    }
    glAttachShader(program, myShader);
    glDeleteShader(myShader);
}
void linkProgram(GLuint program) 
{
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLchar log[1024];
        glGetProgramInfoLog(program, 1024, NULL, log);
        fprintf(stderr, "\n%s\n", log);
        delete[] log;
    }
    glUseProgram(program);
}
void init() 
{
    compileShader(vShaderSrc, GL_VERTEX_SHADER, program);
    compileShader(fShaderSrc, GL_FRAGMENT_SHADER, program);
    linkProgram(program);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static const GLfloat vertex_positions[] = 
    {
            -0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    float currentTime = (float)glfwGetTime();
    float f = currentTime * (float)M_PI * 0.1f;
     mv_matrix = translate(0.0, 0.0, -4.0) * translate(sinf(2.1 * f) * 0.5f, cosf(1.7 * f) * 0.5f, sinf(1.3 * f) * cosf(1.5 * f) * 2.0)  * rotate(currentTime * 45.0f, 0.0f, 1.0f, 0.0f) * rotate(currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
    
    
}

void shutdown() 
{
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
}
void display(GLFWwindow* window) 
{
    const GLfloat color[] = {0.0, 0.5, 0.2, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);
    glUseProgram(program);
    glBindVertexArray(vao);
    GLint uniMv = glGetUniformLocation(program, "mv_matrix");
    glUniformMatrix4fv(uniMv, 1, GL_FALSE, mv_matrix);
    GLint uniProj = glGetUniformLocation(program, "proj_matrix");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj_matrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main (int argc, char** argv)
{
    if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialise GLFW!\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow * window = glfwCreateWindow(800, 600, "Render BG", nullptr, nullptr);
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

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect = width/height;
    mat4 proj_matrix = perspective(50.0f, aspect, 0.1f, 1000.0f);

	while(!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
        display(window);
		glfwSwapBuffers(window);
	}

	shutdown();
	return 0;
}
