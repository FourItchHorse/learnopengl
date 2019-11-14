#define GLEW_STATIC
#include <D:/source/include/glew.h>

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>




class Shader 
{
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);
        void use();
           void setBool(std::string &name, bool value) const;
           void setInt (std::string &name, int value) const;
          void setFloat(std::string &name, float value) const;
        void setVector2(std::string &name, float value1, float value2) const; //these weren't in the tutorial , but added them for completion's sake
        void setVector3(std::string &name, float value1, float value2, float value3) const;
        void setVector4(std::string &name, float value1, float value2, float value3, float value4) const;
};

    Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
    glewExperimental = GL_TRUE;
    glewInit();
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::fstream::failbit | std::fstream::badbit);
    fShaderFile.exceptions (std::fstream::failbit | std::fstream::badbit);
    try 
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        std::cout<<"Writing to vertex shader stream..."<<std::endl;
        fShaderStream << fShaderFile.rdbuf();
        std::cout<<"Writing to fragment shader stream..."<<std::endl;
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode= fShaderStream.str();
    }
    catch(std::ifstream::failure)
    {   
        std::cout<<"ERROR:SHADER FILE NOT SUCCESSFULLY READ"<<std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str(); 
    unsigned int vertex, fragment;
    int status;
    char buffer[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderInfoLog(vertex, GL_COMPILE_STATUS, &status, buffer);
    std::cout << "Compiling vertex shader...\n" << buffer;

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glGetShaderInfoLog(vertex, GL_COMPILE_STATUS, &status, buffer);
    std::cout << "Compiling fragment shader...\n" << buffer;

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    //glBindFragDataLocation(ID, 0, ); this line was included in open.gl but I'm not sure how to implement it here
    glLinkProgram(ID);
    glGetProgramInfoLog(ID, GL_LINK_STATUS, &status, buffer);
    std::cout << "Linking shader program...\n" << buffer;

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    }

    void Shader::use() 
    {
        glUseProgram(ID);
    }

    void Shader::setBool(std::string &name, bool value) const 
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }
    void Shader::setInt(std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void Shader::setFloat(std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void Shader::setVector2(std::string &name, float value1, float value2) const 
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
    }
    void Shader::setVector3(std::string &name, float value1, float value2, float value3) const 
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
    }
    void Shader::setVector4(std::string &name, float value1, float value2, float value3, float value4) const 
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
    }



#endif