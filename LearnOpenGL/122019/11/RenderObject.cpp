#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h> 
#include <GL/glew.h>
#include <STB/stb_image.h>

class renderObject
{
    public:
        renderObject(bool isElement, const char* vShaderSrc, const char* fShaderSrc, const char* texFilePaths[]);
        ~renderObject();
        void Draw();
        void SetVertexAttrib(const char* target, int stepsize, int offset);
        bool isElement;
        static const int vertexAttribCount;
        static const int vertCount;
        static const char* texFilePaths[6];
        GLfloat vertices[];
        GLfloat indices[];
        GLuint* textures[sizeof(texFilePaths)/sizeof(texFilePaths[0])];
    private:
        GLuint loadShader(const char* shaderSrc, GLenum type);
        GLuint initProgram(GLuint vert, GLuint frag);
        void CreateTexture2D(GLuint* texID, const char* filepath, GLenum ActiveTexture); //incl glGenTextures for texID
        void SetDrawObject();
        int mainLoop(SDL_Window* window);
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        GLuint program;
};

renderObject::renderObject(bool isElement, const char* vShaderSrc, const char* fShaderSrc, const char* texFilePaths[]) 
{
    program = initProgram(loadShader(vShaderSrc, GL_VERTEX_SHADER), loadShader(fShaderSrc, GL_FRAGMENT_SHADER));
    for(int i = 0; i < sizeof(texFilePaths)/sizeof(texFilePaths[0]); i++) 
    CreateTexture2D(textures[i], texFilePaths[i], GL_TEXTURE0 + i);
    glewExperimental = GL_TRUE;
    glewInit();
}

int renderObject::mainLoop(SDL_Window* window) 
{

}