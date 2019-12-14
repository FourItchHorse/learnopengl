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
        outColor = vec4(Color, 1.0) * mix(colTex1, colTex2, 0.5);
    }