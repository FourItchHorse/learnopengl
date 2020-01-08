#version 330 core
	in vec2 pos;
	in vec3 colour;
	
	out vec3 Colour;
	void main () 
	{
		Colour = colour;
		gl_position = vec4(position, 0.0, 1.0);
	}
