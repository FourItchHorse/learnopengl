#include <GL/gl3w.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
namespace vmath {
    #include <vmath.h>
};
#include <math.h>
#include <stdio.h>
GLboolean shouldBeRunning = true;
GLuint vao, vbo, program;
GLint mvLoc, prjLoc;
vmath::mat4 mv_matrix, proj_matrix;
GLfloat horizontal, vertical, ticksCount;
void compileShader(GLuint program, const GLchar* source, GLenum type) 
{
    GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &source, NULL);
        glCompileShader(s);
		glAttachShader(program, s);
}
void init() 
{
    	const GLchar* vShaderSrc = R"glsl(
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
			vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
		} 
	)glsl";
	const GLchar* fShaderSrc = R"glsl(
		#version 450 core
		out vec4 color;
		in VS_OUT
		{
			vec4 color;
		} fs_in;
		void main (void) {
			color = fs_in.color;
		}
	)glsl";
	program = glCreateProgram();
	compileShader(program, vShaderSrc, GL_VERTEX_SHADER);
	compileShader(program, fShaderSrc, GL_FRAGMENT_SHADER);
	glLinkProgram(program);
    GLint compiled;
	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
		GLchar Log[1024];
		glGetProgramInfoLog(program, 1024, NULL, Log);
		printf("\n%s\n", Log);
	glUseProgram(program);
	
	mvLoc = glGetUniformLocation(program, "mv_matrix");
	prjLoc = glGetUniformLocation(program, "proj_matrix");
	printf("\n%i, %i \n", mvLoc, prjLoc);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertex_data[] = {
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(vao);

}
void display()
{
    glClearColor(0.0, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glUniformMatrix4fv(prjLoc, 1, GL_FALSE, proj_matrix);
	
	float deltaTime = (float)SDL_GetTicks() * 0.0001f;
    float f = (float)SDL_GetTicks() * 0.00003f;
    

	mv_matrix =  vmath::MAT4_IDENTITY;
    mv_matrix *= vmath::translate(horizontal* 5, -sin(horizontal), 0.0);
	mv_matrix *= vmath::translate(0.0, cos(vertical) * M_PI, sin(vertical));

    for(int i = 0; i < 24; i++) {
    mv_matrix = vmath::translate(0.0f, 0.0, 10.0f - (i * 5)) *
    vmath::rotate(0.0f, 1.0f, 0.0f, deltaTime * 4.5f) * 
	 vmath::rotate(1.0, 0.0, 0.0, deltaTime * 2.1f) *
	 vmath::rotate(0.0, 0.0, 1.0,  deltaTime * (i * 0.1)) * 
		vmath::translate(sinf(2.1 * f) * 2.0f,
		cosf(1.7f * f) * 2.0f,
		sinf(1.3 * f) * cosf(1.5 * f) * 2.0f);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mv_matrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mv_matrix);
	printf("%f, %f\r", horizontal, vertical);
}
void processInput(SDL_Event action)
{
	if(action.type == SDL_QUIT) shouldBeRunning = false; 
	if(action.type == SDL_KEYUP && 
	action.key.keysym.sym == SDLK_ESCAPE) shouldBeRunning = false;
    if(action.type == SDL_KEYDOWN &&
    action.key.keysym.sym == SDLK_w) vertical -= 1.0f;
    if(action.type == SDL_KEYDOWN &&
    action.key.keysym.sym == SDLK_a) horizontal -= 1.0f;
    if(action.type == SDL_KEYDOWN &&
    action.key.keysym.sym == SDLK_s) vertical += 1.0f;
    if(action.type == SDL_KEYDOWN &&
    action.key.keysym.sym == SDLK_d) horizontal += 1.0f;
}
int main (int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO)) 
	{
		printf("Unable to init SDL!\n");
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_Window* window = SDL_CreateWindow("Opengl practice",  SDL_WINDOWPOS_CENTERED, 
				     SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	if(!window)
	{
		printf("Unable to create SDL window! %s\n", SDL_GetError());
		return -1;
	}
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if(gl3wInit()){
		printf("Unable to init opengl context!\n");
		return -1;
	}

    printf("\nGL_VERSION: %s\nGL_VENDOR: %s\nGL_RENDERER: %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	
	init(); 

    GLint width, height;
    SDL_GetWindowSize(window, &width, &height);
    float aspect = (float) width/(float) height;
	proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
	
	SDL_Event event;
	while (shouldBeRunning) 
	{
		if(SDL_PollEvent(&event))processInput(event);
		display();
		SDL_GL_SwapWindow(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
