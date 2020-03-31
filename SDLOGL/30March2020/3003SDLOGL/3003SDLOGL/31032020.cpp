#include "OGLScene.h"
class redGreenBg : public OGLScene
{
public:
	void CompileShader(GLuint program, const GLchar* source, GLenum type) 
	{
		GLuint s = glCreateShader(type);
		glShaderSource(s, 1, &source, NULL);
		glCompileShader(s);
		GLint compiled;
		glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLchar log[1024];
			glGetShaderInfoLog(s, 1024, NULL, log);
			printf("\n%s\n", log);
			delete[] log;
		}
		glAttachShader(program, s);
		glDeleteShader(s);
	}
	void startup()
	{
		const GLchar* vShaderSrc = R"glsl(
		#version 450 core
		layout (location = 0) in vec3 position;
		layout (location = 1) uniform mat4 model;
		layout (location = 2) uniform mat4 view;
		layout (location = 3) uniform mat4 projection;		
		out VS_OUT
		{
			vec4 color;
		}vs_out;
		void main (void) {
				gl_Position = projection * view  * model * vec4(position, 1.0f);
				vs_out.color = vec4(position * 1.0f) * 2.0;
		}
	)glsl";
		const GLchar* fShaderSrc = R"glsl(
		#version 450 core
		out vec4 outCol;
		in VS_OUT
		{
			vec4 color;
		}fs_in;
		void main (void) 
		{
			outCol = fs_in.color;
		}
		)glsl";
		
		program = glCreateProgram();
		CompileShader(program, vShaderSrc, GL_VERTEX_SHADER);
		CompileShader(program, fShaderSrc, GL_FRAGMENT_SHADER);
		glLinkProgram(program);
		GLint compiled;
		glGetProgramiv(program, GL_LINK_STATUS, &compiled);
		if (!compiled)
		{
			GLchar log[1024];
			glGetProgramInfoLog(program, 1024, NULL, log);
			printf("\n%s\n", log);
		}
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		static const GLfloat vertex_positions[] =
		{
			-0.25f,  0.25f, -0.25f,
			-0.25f, -0.25f, -0.25f,
			 0.25f, -0.25f, -0.25f,

			 0.25f, -0.25f, -0.25f,
			 0.25f,  0.25f, -0.25f,
			-0.25f,  0.25f, -0.25f,

			 0.25f, -0.25f, -0.25f,
			 0.25f, -0.25f,  0.25f,
			 0.25f,  0.25f, -0.25f,

			 0.25f, -0.25f,  0.25f,
			 0.25f,  0.25f,  0.25f,
			 0.25f,  0.25f, -0.25f,

			 0.25f, -0.25f,  0.25f,
			-0.25f, -0.25f,  0.25f,
			 0.25f,  0.25f,  0.25f,

			-0.25f, -0.25f,  0.25f,
			-0.25f,  0.25f,  0.25f,
			 0.25f,  0.25f,  0.25f,

			-0.25f, -0.25f,  0.25f,
			-0.25f, -0.25f, -0.25f,
			-0.25f,  0.25f,  0.25f,

			-0.25f, -0.25f, -0.25f,
			-0.25f,  0.25f, -0.25f,
			-0.25f,  0.25f,  0.25f,

			-0.25f, -0.25f,  0.25f,
			 0.25f, -0.25f,  0.25f,
			 0.25f, -0.25f, -0.25f,

			 0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f,  0.25f,

			-0.25f,  0.25f, -0.25f,
			 0.25f,  0.25f, -0.25f,
			 0.25f,  0.25f,  0.25f,

			 0.25f,  0.25f,  0.25f,
			-0.25f,  0.25f,  0.25f,
			-0.25f,  0.25f, -0.25f
		};
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(vbo, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	void render(float currentTime)
	{
		glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
		glViewport(0,0, info.windowWidth, info.windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		for (size_t i = 0; i < 24; i++)
		{
			 mat4 modelMatrix =  translate(0.0, 0.0, -1.0f * i) *  translate(0.0, sin(currentTime), 0.0);
			glUniformMatrix4fv(1, 1, GL_FALSE, modelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		 mat4 viewMatrix =  translate(sin(horizontal), cos(horizontal), 0.0f) *  translate(0.0f, cos(vertical), sin(vertical) * M_PI);
	}

private:
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLfloat horizontal;
	GLfloat vertical;
};

int main(int argc, char** argv)
{
	redGreenBg* app = new redGreenBg();
	app->run(app);
	delete app;
	return 0;
} 