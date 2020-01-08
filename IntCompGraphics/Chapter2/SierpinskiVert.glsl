#version 330 core
            in vec2 pos;
	     in vec3 col;
		out vec3 Colour;
	main() {

		Colour = col;
		gl_Position = vec4(pos, 0.0, 1.0);
	}
