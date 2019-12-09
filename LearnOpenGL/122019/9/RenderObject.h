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

class RenderObject 
{
    public:
    RenderObject(const GLint contextAttributes[], GLint windowAttributes[], const char* title, bool isElement);
    ~RenderObject();

    const char* vShaderSrcs[];
    const char* fShaderSrcs[];

    void CreateTexture2D(const char* filepath, GLenum activeTexture, GLuint textureID); //add glGenTextures to this as well this time
    GLuint textures[];

    GLfloat vertices[];
    GLuint indices[];

    GLint Draw();

    private:
    GLvoid loadShader(const char* shaderSrc);
    GLuint initShaderProgram(const char* vertexSrc, const char* fragmentSrc);
    
    GLint SetVertexAttrib(GLuint program, const char* target, GLint stepsize, GLint vertexAttribSize, GLint offset);

    GLvoid setElementDrawObject(GLfloat vertices[], GLuint indices[], GLuint vao[], GLuint vbo[], GLuint ebo[]);
    GLvoid setArrayDrawObject(GLfloat vertices[], GLuint vao[], GLuint vbo[]);

    glm::mat4 modelTransform(SDL_Event windowEvent, const uint8_t* keyboardState);
    glm::mat4 viewTransform(SDL_Event windowEvent, const uint8_t* keyboardState);
    glm::mat4 projTransform(SDL_Event windowEvent, const uint8_t* keyboardState);

    GLvoid drawArrayObject(GLuint shader, GLuint vao, GLuint vertCount); //goes through a while(true) loop and returns 0 when broken
    GLvoid drawElementObject(GLuint shader, GLuint vao, GLuint ebo, GLuint vertCount);

    GLuint shaderProgram[];

    GLuint vao[];
    GLuint vbo[];
    GLuint ebo[];
};