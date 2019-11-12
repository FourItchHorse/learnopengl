#ifndef SHADER_H
#define SHADER_H

#include <D:/source/include/glew.h>

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
};

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
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
        fShaderStream << fShaderStream.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode= fShaderStream.str();
    }
    catch (std::fstream::failure e)
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

    void Shader::setBool(const std::string name, bool value) 
    {
        glGetUniformLocation();
    }
    void Shader::setInt(const std::string name, int value) 
    {

    }
    void Shader::setFloat(const std::string name, float value) 
    {

    }

#endif