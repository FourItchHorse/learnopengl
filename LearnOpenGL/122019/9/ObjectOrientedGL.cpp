#include "RenderObject.h"

static const GLint SpinningCubeCA[] = 
{
    SDL_GL_CONTEXT_PROFILE_MASK, 
    SDL_GL_CONTEXT_PROFILE_CORE,
    SDL_GL_CONTEXT_MAJOR_VERSION,
    3,
    SDL_GL_CONTEXT_MINOR_VERSION,
    3,
    SDL_GL_STENCIL_SIZE,
    8
};

int main () 
{
    RenderObject* spinningCube = new RenderObject(SpinningCubeCA, "Spinning Cube", false);
    spinningCube->Draw();
    spinningCube->~RenderObject();
    return 0;
}