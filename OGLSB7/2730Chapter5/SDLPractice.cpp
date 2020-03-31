#include <GL/gl3w.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
GLboolean shouldBeRunning = true;
GLuint vao, vbo, program, horizontal, vertical;
void compileShader() 
{

}
void init() 
{
}
void display()
{
}
void processInput(SDL_Event action)
{
	if(action.type == SDL_QUIT) shouldBeRunning = false; 
	if(action.type == SDL_KEYUP && 
	action.key.keysym.sym == SDLK_ESCAPE) shouldBeRunning = false;
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

	SDL_Event event;
	while (shouldBeRunning) 
	{
		if(SDL_PollEvent(&event))processInput(event);
		display();
		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
