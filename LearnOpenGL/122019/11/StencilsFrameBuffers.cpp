#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h> 
#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Context 
{   
    public:
    Context(int contextAttr[],const char* title);
    ~Context();
    SDL_Window* window;
    SDL_GLContext context;
    const char* title;
};

Context::Context(int contextAttr[],const char* title) 
{
       for(int i = 0; i < 7; i++)
        SDL_GL_SetAttribute((SDL_GLattr) contextAttr[i], contextAttr[i + 1]);
        window = SDL_CreateWindow(title, contextAttr[8], contextAttr[9], contextAttr[10], contextAttr[11], contextAttr[12]);
        context = SDL_GL_CreateContext(window);
        glewExperimental = GL_TRUE;
        glewInit();
}
Context::~Context() 
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int myCA[] {
    SDL_GL_CONTEXT_PROFILE_MASK,
    SDL_GL_CONTEXT_PROFILE_CORE,
    SDL_GL_CONTEXT_MAJOR_VERSION,
    3,
    SDL_GL_CONTEXT_MINOR_VERSION,
    3,
    SDL_GL_STENCIL_SIZE,
    8,
    200,
    100,
    800, 
    600, 
    SDL_WINDOW_OPENGL
};

 int main () 
 {
    Context* myContext = new Context(myCA, "Stencils and Framebuffers");
    SDL_Event windowEvent;
   
    
 }