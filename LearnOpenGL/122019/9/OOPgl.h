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

#include <iostream>
#include <vector>

struct renderObj{

        GLboolean isElement;
        const char* fShaderSrc;
        const char* vShaderSrc;
        const char* imageFilePaths[];
        GLuint vertexAttribCount;
        GLfloat* vertices[];
        GLuint* indices[];
        GLuint* textures[];
};

class RenderObject 
{
    
    public:
    RenderObject(renderObj renderObject);
    ~RenderObject();

    renderObj myRender;

    GLuint initShaderProgram(const char* vertexSrc, const char* fragmentSrc);

    GLuint* createTexture2D(const char* filepath, GLenum activeTexture); //add glGenTextures to this as well this time

    GLint setVertexAttrib(const char* target, GLint stepsize, GLint offset);

    glm::mat4 modelTransform(SDL_Event windowEvent, const uint8_t* keyboardState);
    glm::mat4 viewTransform(SDL_Event windowEvent, const uint8_t* keyboardState);
    glm::mat4 projTransform(SDL_Event windowEvent, const uint8_t* keyboardState);

    GLvoid Draw();

    private:
    GLuint loadShader(const char* shaderSrc, GLenum type);

    GLvoid setElementDrawObject(GLfloat vertices[], GLuint indices[]);
    GLvoid setArrayDrawObject(GLfloat vertices[]);

    GLint drawArrayObject(GLuint vertCount); //goes through a while(true) loop and returns 0 when broken
    GLint drawElementObject(GLuint vertCount);

    GLuint shaderProgram;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};


class Context 
{
    public: 
    Context(const GLint contextAttributes[], GLint windowAttributes[], const char* title);
    ~Context();
    std::vector<RenderObject*> renderObjects;
    private:
    SDL_Window* window;
    SDL_GLContext context;
};