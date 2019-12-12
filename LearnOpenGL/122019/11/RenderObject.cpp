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
#include <iterator>

class renderObject
{
    public:
        renderObject(bool isElement, const char* vShaderSrc, const char* fShaderSrc, char* texFilePaths[], GLfloat vertices[], GLuint indices[]);
        ~renderObject();
        void Draw();
        void SetVertexAttrib(const char* target, int stepsize, int offset);
        virtual glm::mat4 modelTransform();
        virtual glm::mat4 viewTransform();
        virtual glm::mat4 projTransform(); 
        bool isElement;
        static const int vertexAttribCount;
        static const int vertCount;
        char* texFilePaths[6];
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
        GLfloat vertices[];
        GLfloat indices[];
};

renderObject::renderObject(bool isElement, const char* vShaderSrc, const char* fShaderSrc, char* myTexFilePaths[], GLfloat verts[], GLuint inds[]) 
{
    program = initProgram(loadShader(vShaderSrc, GL_VERTEX_SHADER), loadShader(fShaderSrc, GL_FRAGMENT_SHADER));
    for(int i = 0; i < sizeof(myTexFilePaths)/sizeof(myTexFilePaths[0]); i++) 
    CreateTexture2D(textures[i], texFilePaths[i], GL_TEXTURE0 + i);
    glewExperimental = GL_TRUE;
    glewInit();
    this.vertices = verts;
    this.indices = inds;
}

void SetDrawObject() 
{

}

int renderObject::mainLoop(SDL_Window* window) 
{
    return 0;
}