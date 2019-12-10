/*NOTE TO SELF: Create function for load shader, load texture2D processing input, updating and generating output
Brian Beuken chapter 5, Invaders, has Gluint functions LoadShader and LoadProgram. Brian Beuken's Init simply stores
the vertex and fragment shader source, loads the program from said sources, sets the attributes and uniforms (which would be useful).
Probably best to keep the vbo, ebo and vao in main*/


#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include <SDL2/SDL.h>
#include <SDL2\SDL_opengl.h>

#include <STB/stb_image.h>

#include <glm\glm.hpp>
#include <glm/gtc\matrix_transform.hpp>
#include <glm/gtc\type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


const char* vShaderSrc = R"glsl(
     #version 330 core
    
    in vec3 position;
    in vec3 color;
    in vec2 texcoord1;

    out vec3 Color;
    out vec2 Texcoord1;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main () 
    {
         Color = color;
        Texcoord1 = texcoord1;

        gl_Position = proj * view * model * vec4(position, 1.0);
    }
)glsl";
const char* fShaderSrc = R"glsl(
    #version 330 core
    
    in vec3 Color;
    in vec2 Texcoord1;

    out vec4 outColor;

    uniform sampler2D tex1;
    uniform sampler2D tex2;

    void main () 
    {
        vec4 colTex1 = texture(tex1, Texcoord1);
        vec4 colTex2 = texture(tex2, Texcoord1);
        outColor = mix(colTex1, colTex2, 0.5) * (vec4(Color, 1.0));
    }
)glsl";


glm::mat4 modelTransform(float WND_WIDTH, float WND_HEIGHT, SDL_Event windowEvent, const uint8_t * keyboardState) //added event and keyboard states as args so I can recieve user input 
{
    float time = SDL_GetTicks()/512.0f;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f));
    model = glm::rotate(model, glm::radians(20.0f * time), glm::vec3(0.0f, 0.0f, 1.0f));
    return model;
}

glm::mat4 viewTransform(float WND_WIDTH, float WND_HEIGHT, SDL_Event windowEvent, const uint8_t * keyboardState) 
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    float mouseX = (static_cast<float>(x) - WND_WIDTH/2.0f)/WND_WIDTH;
    float mouseY = (static_cast<float>(y) - WND_HEIGHT/2.0f) /WND_HEIGHT;
    glm::mat4 view = glm::lookAt(
        glm::vec3(1.2f * (mouseX * 15.0f), 5.0f, 2.0f * (mouseY * 5)),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );  
    return view;
}

glm::mat4 projTransform(float WND_WIDTH, float WND_HEIGHT, SDL_Event windowEvent, const uint8_t * keyboardState) 
{
    glm::mat4 proj = glm::mat4(1.0f);
        proj = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(WND_WIDTH)/static_cast<float>(WND_HEIGHT),
            1.0f,
            100.0f
        );
    return proj;
}


static const GLint context_attributes[] = 
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

void CreateTexture2D(const char* filepath, GLenum activeTexture, GLuint textureID)
{
     glActiveTexture(activeTexture);
        glBindTexture(GL_TEXTURE_2D, textureID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    GLint width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        std::cout << "FAILED TO GENERATE TEXTURE\n";
    }
    stbi_image_free(data);
}

GLuint LoadShader(GLenum type, const char* shaderSrc) 
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

GLuint InitProgramObj(const char* vShaderSrc, const char* fShaderSrc)
{
    GLuint vertexShader, fragmentShader, programObject;
       vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderSrc);
       fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderSrc);

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

GLint SetVertexAttrib (GLuint shaderProgram, const char* target, GLint attribsize, GLint stepSize, GLint offset) 
{
      GLint myAttrib = glGetAttribLocation(shaderProgram, target);
        glEnableVertexAttribArray(myAttrib);
        glVertexAttribPointer(myAttrib, attribsize, GL_FLOAT, GL_FALSE, stepSize * sizeof(GLfloat), (void*) (offset * sizeof(GLfloat)));
        std::cout << target << " attrib error " << glGetError() << std::endl;
        return myAttrib;
} 

int main() 
{
    SDL_Init(SDL_INIT_VIDEO);
    for(int i = 0; i < sizeof(context_attributes)/sizeof(context_attributes[0]); i += 2) 
    {
        SDL_GL_SetAttribute((SDL_GLattr) context_attributes[i], context_attributes[i + 1]);
    }
    
    SDL_Window* window = SDL_CreateWindow("Organised cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();
    std::cout << "INIT ERRROR " << glGetError() << std::endl;

    GLuint shaderProgram = InitProgramObj(vShaderSrc, fShaderSrc); 
    glUseProgram(shaderProgram);
    std::cout << "SHADER ERRROR " << glGetError() << std::endl;

    GLuint vao, vbo, textures[2];
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenTextures(2, textures);
        GLfloat vertices[] 
    {
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 
    };

    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    CreateTexture2D("Textures\\grapefruit.png", GL_TEXTURE0, textures[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0); 
    CreateTexture2D("Textures\\Lenna.png", GL_TEXTURE1, textures[1]);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1); 
     
    
    GLint vertexAttribSize = 8;

    GLint posAttrib = SetVertexAttrib(shaderProgram, "position", 3, vertexAttribSize, 0); 
    GLint colAttrib = SetVertexAttrib(shaderProgram, "color", 3, vertexAttribSize, 3);
    GLint texAttrib = SetVertexAttrib(shaderProgram, "texcoord1", 2, vertexAttribSize, 6);

    glEnable(GL_DEPTH_TEST); 

    SDL_Event windowEvent;
    const uint8_t * keyboardState = SDL_GetKeyboardState(0);
    while (true)
    {
        glBindVertexArray(vao);
        glUseProgram(shaderProgram);

        if(SDL_PollEvent(&windowEvent)) 
        {
            if(windowEvent.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE])
            break; 
        }
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = projTransform(800, 600, windowEvent, keyboardState);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glm::mat4 view = viewTransform(windowEvent, keyboardState);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = modelTransform(windowEvent, keyboardState); 
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        SDL_GL_SwapWindow(window);
    }

    
    glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);    

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
} //Have managed to shed over 100 lines of code compared to InteractiveCube which is nice :)