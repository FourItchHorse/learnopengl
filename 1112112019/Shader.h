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
        fragmentCode = fShaderStream.str();
    }
 
}

#endif