#version 330 core
in vec2 pos;
in vec3 col;
out vec4 Colour;
void main() 
{
   Colour = vec4(col, 1.0);
   gl_Position = vec4(pos, 0.0, 1.0);
}
