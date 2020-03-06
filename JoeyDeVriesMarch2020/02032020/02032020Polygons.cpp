#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

typedef struct { 
        GLenum		type;
	const char* filename;
	GLuint 	      shader;
} ShaderInfo;

	/*code from github.com/openglredbook/examples/blob/master/lib/LoadShaders.cpp*/
static const GLchar* ReadShader(const char* filename) 
{
	FILE* infile = fopen (filename, "rb" );
	if(!infile) 
	{ 
		std::cerr << "Unable to open file '"<< filename << ",'" << std::endl;
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	int len = ftell (infile);
	fseek (infile, 0, SEEK_SET);
	GLchar* source = new GLchar[len + 1];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;
	printf(source);
	return const_cast<const GLchar*>(source);
}
GLuint LoadShaders(ShaderInfo* shaders) 
{

	if( shaders == NULL ) {return 0; }
	GLuint program = glCreateProgram();
	ShaderInfo* entry = shaders;
	while( entry->type != GL_NONE) 
	{
		GLuint shader = glCreateShader(entry->type);
		entry->shader = shader;
		const GLchar* source = ReadShader(entry-> filename);
		if ( source  == NULL ) {
		for(entry = shaders; entry->type != GL_NONE; ++entry) {
				glDeleteShader( entry->shader );
				entry->shader = 0;
			}
			return 0;
		}
	glShaderSource(shader, 1, &source, NULL);
	delete [] source;

	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled );
	if (! compiled ) {
		
			GLsizei len;
			GLchar* log = new GLchar[len+1];
			glGetShaderInfoLog(shader, len, &len, log);
				std::cerr << "Shader compilation failed :" << log << std::endl;
			delete [] log;
		

			return 0;
	}
	glAttachShader( program, shader);

	++entry;
	}
	glLinkProgram( program );

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked) 
	{
	
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len );
		GLchar* log = new GLchar[len +1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
	
	
	
	for(entry = shaders; entry->type != GL_NONE; ++entry) 
	{
		glDeleteShader(entry->shader);
		entry->shader = 0;
	}
		return 0;
	}
	return program;
}


enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ElementBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColour = 1};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init (void) 
{
	static const GLfloat vertices[NumVertices][5] =
	{
		{-0.9, -0.90},
		{0.85, -0.90},
		{-0.9, 0.85},
		
		{0.9, -0.85},
		{0.9, 0.90},
		{-0.85, 0.90}
	};


	glCreateVertexArrays(NumVAOs, VAOs);

	glCreateBuffers(NumBuffers, Buffers);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "02032020.vert" },
		{GL_FRAGMENT_SHADER, "02032020.frag" },
		{GL_NONE, NULL }
	};


        GLuint program = LoadShaders(shaders);

	glUseProgram(program);

	glBindVertexArray(VAOs[Triangles]);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(vPosition);

}
void display(void)
{
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int main(int argc, char** argv) 
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Polygons", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(!glewInit()) { fprintf(stderr, "Unable to init GLEW"); }
	
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
