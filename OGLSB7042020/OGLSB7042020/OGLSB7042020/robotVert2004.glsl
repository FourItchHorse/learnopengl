#version 450 core
layout (location = 0) in vec4 position;
out VS_OUT 
{
  vec4 outcol;
} vs_out;
uniform mat4 prj_matrix;
uniform mat4 mv_matrix;
void main (void)
{
    gl_Position = prj_matrix * mv_matrix * position;
    vs_out.outcol =  position * mv_matrix;
}