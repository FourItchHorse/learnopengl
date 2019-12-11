#include "renderObject.cpp"

class Context 
{   
    public:
    Context();
    ~Context();
    SDL_Window* window;
    SDL_GLContext context;
    static const int contextAttr[8];
    const char* title;
    int x, y, w, h, windowType;
};

Context::Context() 
{
       for(int i = 0; i < sizeof(contextAttr)/ sizeof(contextAttr[0]) - 1; i++)
        SDL_GL_SetAttribute((SDL_GLattr) contextAttr[i], contextAttr[i + 1]);
        window = SDL_CreateWindow(title, x, y, w, h, windowType);
        context = SDL_GL_CreateContext(window);
  
}

 int main () 
 {
        
 }