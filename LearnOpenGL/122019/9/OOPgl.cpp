#include "OOPgl.h"

Context::Context(const GLint contextAttributes[], GLint windowAttributes[], const char* title) 
{
    SDL_Init(SDL_INIT_VIDEO);
    for(int i = 0; i < sizeof(contextAttributes)/sizeof(contextAttributes[0]); i += 2) 
    {
        SDL_GL_SetAttribute((SDL_GLattr) contextAttributes[i], contextAttributes[i + 1]);
    }
    window = SDL_CreateWindow(title, windowAttributes[0], windowAttributes[1], windowAttributes[2], windowAttributes[3], windowAttributes[4]);
    context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();
    std::cout << "INIT ERRROR " << glGetError() << std::endl;

    for(auto iter = renderObjects.begin; iter != renderObjects.end; iter++)
    renderObjects[iter]->RenderObject(); 
}

Context::~Context() 
{
    for(auto iter = renderObjects.begin; iter != renderObjects.end; iter++) 
    renderObjects[iter]->~RenderObject();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

RenderObject::RenderObject( renderObj renderObject) 
{
    this->myRender = renderObject; 
    this->shaderProgram = initShaderProgram(myRender.vShaderSrc, myRender.fShaderSrc);
    std::cout << "SHADER ERROR" << glGetError() << std::endl;
    for(int i = 0; i < sizeof(myRender.textures)/sizeof(myRender.textures[0]); i++)
    myRender.textures[i] = createTexture2D(myRender.imageFilePaths[i], GL_TEXTURE0 + i);
    std::cout << "TEXTURE ERROR" << glGetError() << std::endl;
}

RenderObject::~RenderObject() 
{   
    glDeleteProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glDeleteBuffers(1, &vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    glDeleteBuffers(1, &ebo);
    
    glBindVertexArray(0); 
    glDeleteVertexArrays(1, &vao);
}



GLvoid RenderObject::Draw() 
{
    GLuint myVertCount = 0;
    if (myRender.isElement) {
        myVertCount = sizeof(myRender.indices)/sizeof(myRender.indices[0]) / myRender.vertexAttribCount;
        setElementDrawObject(myRender.vertices, myRender.indices);
        drawElementObject(myVertCount);
    }
    else if (myRender.isElement == false) 
    {
        GLuint myVertCount = sizeof(myRender.vertices)/sizeof(myRender.vertices[0]) / myRender.vertexAttribCount;
        setArrayDrawObject(myRender.vertices);
        drawArrayObject(myVertCount);
    }
}

GLuint RenderObject::loadShader(const char* shaderSrc, GLenum type) 
{
    GLuint myShader = glCreateShader(type);
    
    if (myShader == 0) return 0;
    glShaderSource(myShader, 1, &shaderSrc, NULL);

    glCompileShader(myShader);
    GLint compiled;
    glGetShaderiv(myShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) 
    {
        char buffer[512];
        glGetShaderInfoLog(myShader, 512, NULL, buffer);
        fprintf(stderr, "FAILED TO COMPILE\n%s\n", buffer);
        glDeleteShader(myShader);
        return 0;
    }

    return myShader;
}

GLuint RenderObject::initShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
     GLuint vertexShader, fragmentShader, programObject;
       vertexShader = loadShader(vertexSrc, GL_VERTEX_SHADER);
       fragmentShader = loadShader(fragmentSrc, GL_FRAGMENT_SHADER);

       programObject = glCreateProgram();
       if(programObject == 0) return false;

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    
    if(!linked)
        {
            GLchar buffer[512];
            glGetProgramInfoLog(programObject, 512, NULL, buffer);
            fprintf(stderr, "FAILED TO LINK\n%s\n", buffer);
            return false;
        }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return programObject;
}

