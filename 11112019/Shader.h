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

}

#endif