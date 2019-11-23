#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include <SDL2/SDL.h>
#include <SDL2\SDL_opengl.h>

#include <STB/stb_image.h>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>

const GLchar* vertexShaderSource = R"glsl(

)glsl";
const GLchar* fragmentShaderSource = R"glsl(

)glsl";

int main () 
{
    SDL_Event windowEvent;
    while (true)
    { 
        if(SDL_PollEvent(&windowEvent)) 
        {
            if (windowEvent.type == SDL_QUIT) 
            {
                break;
            }
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                break;
            }
        }
    }
    
}