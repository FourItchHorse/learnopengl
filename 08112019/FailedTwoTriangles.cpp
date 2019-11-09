#include <D:/source/include/glad.h> //starting from scratch without copying to revise concepts
#include <D:/source/include/glfw3.h>

#include <iostream>

//goal: create two triangles, one blue one yellow with different VAOs and VBOS (and two different fragment shaders)
GLFWwindow* createContextAndWindow(int width, int height, const char* title);
unsigned int compileShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330\n"
"layout (location = 0) in vec3 aPos;\n" 
"void main()" 
"{" 
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
"}";

const char* fragmentShaderBlue = "#version 330\n"
"out vec4 FragColor;"
"void main() "
"{ FragColor = vec4(0.0, 0.0, 0.7, 1.0);"
" }";

const char* fragmentShaderYellow = "#version 330\n"
"out vec4 FragColor;"
"void main() {"
" FragColor = vec4(0.7f, 0.7f, 0.0f, 1.0f);"
"}";

int main() 
{
    glfwInit();
    
    GLFWwindow* window = createContextAndWindow(800,600,"Two different colour triangles");

    unsigned int blueShader = compileShaderProgram(vertexShaderSource, fragmentShaderBlue);
    unsigned int yellowShader = compileShaderProgram(vertexShaderSource, fragmentShaderYellow);

    float triangleY[] 
    {
        -0.25, 1.0, 0.0,
        -1.0, -0.5, 0.0, 
        -0.5, -0.5, 0.0
    };
    float triangleB[]
    {
        0.25, 1.0, 0.0, 
        0.25, -0.5, 0.0,
        0.5, -0.5, 0.0f,
    };

    unsigned int VBOy, VBOb, VAOy, VAOb;
    glGenVertexArrays(1, &VAOy);
    glGenBuffers(1, &VBOy);
    glBindVertexArray(VAOy);

    glGenVertexArrays(1, &VAOb);
    glGenBuffers(1, &VBOb);
    glBindVertexArray(VAOb);

    
    glBindBuffer(GL_ARRAY_BUFFER, VBOy);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleY), triangleY, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBOb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleB), triangleB, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2,0.2,0.2,1.0); //rendered in a different colour         
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(yellowShader);
        glBindVertexArray(VAOy);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(blueShader);
        glBindVertexArray(VAOb);
        glDrawArrays(GL_TRIANGLES, 2, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    glfwTerminate();
    return 0;
}


GLFWwindow* createContextAndWindow(int width, int height, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(window == NULL) 
    {
        std::cout<< "FAILED TO CREATE GLFW WINDOW" <<std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout<<"FAILED TO INITIALIZE GLAD"<<std::endl;
        glfwTerminate();
        return nullptr;
    }

    glViewport(0,0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    return window;
}

unsigned int compileShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) 
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout<<"ERROR: VERTEX SHADER COMPILATION FAILED\n"<< infoLog << std::endl;
        return -1;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout<<"ERROR: FRAGMENTSHADER COMPILATION FAILED\n" << infoLog << std::endl;
        return -1;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"ERROR: SHADER LINKING FAILED" << infoLog << std::endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void processInput(GLFWwindow* window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0,0,width, height);
}