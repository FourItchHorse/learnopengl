#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL_opengl.h>
namespace vmath {
#include <vmath>};
#include <math.h>
#include <chrono>
class OGLScene 
{
	struct {
	 
	} contextInfo; 
}
int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SD

	SDL_Quit();
	return 0;
}
