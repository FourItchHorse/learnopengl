#include "OOPgl.h"

const char* vertexShaderSource =  
R"glsl(
#version 330 core
in vec2 position;
in vec3 color;
in vec2 texcoord1;
in vec2 texcoord2;

out vec3 Color;
out vec2 TexCoord1;
out vec2 TexCoord2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main () 
{
    Color = color;
    TexCoord1 = texcoord1;
    TexCoord2 = texcoord2;
    
    gl_Position = proj * view  * model * vec4(position, 0.0, 1.0);
}
)glsl"
;

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 Color;
in vec2 TexCoord1;
in vec2 TexCoord2;

out vec4 outColor;

uniform float time;
uniform mat4 colorEffect;
uniform mat4 texEffect;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main () 
{
    vec4 colTex1 = texture(tex1, TexCoord1);
    vec4 colTex2 = texture(tex2, TexCoord2 + time) * texEffect;
    outColor = mix(colTex1, colTex2, 0.5) / (colorEffect * vec4(Color, 1.0));
}
)glsl"


static const GLint SCCA[] = 
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

static GLint SCWA [] =
{
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 
    800, 
    600
};

static const char* SCFP[] = 
{
 "Textures/butterfly.png",
 "Textures/cat.png",
 "Textures/container.png",
 "Textures/Lenna.png"
};

int main () 
{
    RenderObject* spinningCube = new RenderObject( false, 
    vertexShaderSources, fragmentShaderSources, SCFP);
    GLuint SCTex[4];
    spinningCube->Draw();
    spinningCube->~RenderObject();
    return 0;
}