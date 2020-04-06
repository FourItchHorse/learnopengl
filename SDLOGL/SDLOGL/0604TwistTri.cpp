#include "OGLScene.h"
#include <math.h>
class Sierpinski2D : public OGLScene
{
	const  int NDivisions = 5;
	const  static int NumVertices = 729;
	vec2 points[NumVertices];

        int i = 0;	
	void triangle(vec2 &a, vec2 &b, vec2 &c) 
	{
		double d;
		vec2 e;
		d = sqrt(a[0]*a[0] + a[1] * a[1]);
		e[0] = cos(angle * d)*a[0] - sin(angle*d)*a[1];
		e[1] = sin(angle * d)*a[0] - cos(angle*d)*a[1];
		points[i++] = e;
		d = sqrt(b[0]*b[0] + b[1] * b[1]);
		e[0] = cos(angle * d)*b[0] - sin(angle*d)*b[1];
		e[1] = sin(angle * d)*b[0] - cos(angle*d)*b[1];
		points[i++] = e; 
		d = sqrt(c[0]*c[0] + c[1] * c[1]);
		e[0] = cos(angle * d)*c[0] - sin(angle*d)*c[1];
		e[1] = sin(angle * d)*c[0] - cos(angle*d)*c[1];	
		points[i++] = e;
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
		layout (location = 1) uniform mat4 rotation;
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
		
		v[3] = {vec2(-1.0, -1.0), vec2(-0.0, 1.0), vec2(1.0, -1.0)};

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
	
		glCreateBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

		compileShader(program, vShaderSource, GL_VERTEX_SHADER);
		compileShader(program, fShaderSource, GL_FRAGMENT_SHADER);
		glLinkProgram(program);
		glUseProgram(program);

		GLuint loc = glGetAttribLocation(program, "vPosition");
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		glEnableVertexAttribArray(loc);
		
		glClearColor(0.0, 0.5, 0.5, 1.0);
	}
	void processInput(SDL_Event action) 
	{
		if(action.type == SDL_KEYDOWN && action.key.keysym.sym == SDLK_s)
								   angle += 1.0f;
		if(action.type == SDL_KEYDOWN && action.key.keysym.sym == SDLK_w)
								   angle -= 1.0f;


	}
	void render(float currentTime)
	{
		i = 0;
		divide_triangle(v[0], v[1], v[2], NDivisions);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);
		glBindVertexArray(vao);
		glUseProgram(program);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

private:
	int i;
	GLuint vao;
	GLfloat angle;
	GLuint buffer;
	GLuint texture;
	GLuint program;
	vec2 v[];
};

DECLARE_MAIN(Sierpinski2D);

int main(int argc, char** argv) 
{
	Sierpinski2D* app = new Sierpinski2D();
	app->run(app);
	delete app;
	return 0;
} 
