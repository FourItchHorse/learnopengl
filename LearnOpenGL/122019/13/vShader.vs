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