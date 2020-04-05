#include "OGLScene.h"

class Sierpinski2D : public OGLScene
{
	const  int NDivisions = 5;
	const  static int NumVertices = 729;
	vec2 points[NumVertices];

	int i = 0;
	void triangle(vec2 &a, vec2 &b, vec2 &c) 
	{
		points[i++] = a;
		points[i++] = b;
		points[i++] = c;
	}
	void divide_triangle(vec2 &a, vec2 &b, vec2 &c, int k) 
	{
		if (k > 0)
		{
			vec2 ab = (a + b) / 2.0;
			vec2 ac = (a + c) / 2.0;
			vec2 bc = (b + c) / 2.0;

			divide_triangle(a, ab, ac, k - 1);
			divide_triangle(c, ac, bc, k - 1);
			divide_triangle(b, bc, ab, k - 1);
		}
		else triangle(a, b, c);
	}
	void startup()
	{
		program = glCreateProgram();
		const GLchar* vShaderSource = R"glsl(
		#version 450 core
		in vec2 vPosition;
		out VS_OUT
		{
			out vec4 color;
		}vs_out;
		void main()
		{
			gl_Position = vec4(vPosition, 0.5, 1.0);
			vs_out.color = gl_Position * 2.0 + vec4(0.5, 0.5, gl_VertexID, 0.0);
		}
		)glsl";
		const GLchar* fShaderSource = R"glsl(
		#version 450 core
		out vec4 color;	
		in VS_OUT
		{
			in vec4 color;
		}fs_in;
		void main () 
		{
			color = fs_in.color;
		}
		)glsl";
		
		vec2 v[3] = {vec2(-1.0, -1.0), vec2(-0.0, 1.0), vec2(1.0, -1.0)};
		divide_triangle(v[0], v[1], v[2], NDivisions);

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
	
		glCreateBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		compileShader(program, vShaderSource, GL_VERTEX_SHADER);
		compileShader(program, fShaderSource, GL_FRAGMENT_SHADER);
		glLinkProgram(program);
		glUseProgram(program);

		GLuint loc = glGetAttribLocation(program, "vPosition");
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		glEnableVertexAttribArray(loc);
		glClearColor(0.0, 0.5, 0.5, 1.0);
	}
	void render(float currentTime)
	{
		glBindVertexArray(vao);
		glUseProgram(program);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

private:
	GLuint vao;
	GLuint buffer;
	GLuint texture;
	GLuint program;
};

//DECLARE_MAIN(Sierpinski2D);
/*
int main(int argc, char** argv) 
{
	Sierpinski2D* app = new Sierpinski2D();
	app->run(app);
	delete app;
	return 0;
} */
