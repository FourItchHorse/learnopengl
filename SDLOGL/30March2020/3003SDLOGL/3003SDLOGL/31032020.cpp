#include "OGLScene.h"
class flyingCubes : public OGLScene 
{
public:
	void compileShader(GLuint program, const GLchar* source, GLenum type) {
		GLuint s = glCreateShader(type);
		glShaderSource(s, 1, &source, NULL);
		glCompileShader(s);
		glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
		glAttachShader(program, s);
	}
	virtual void startup() 
	{
		const GLchar* vShaderSrc = R"glsl(
		#version 450 core
		in vec4 position;
		out VS_OUT
		{
			vec4 color;
		} vs_out;
		uniform mat4 model_matrix;
		uniform mat4 view_matrix;
		uniform mat4 proj_matrix;
		void main(void)
		{
			gl_Position = proj_matrix * view_matrix * model_matrix * position;
			vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
		} 
	)glsl";
		const GLchar* fShaderSrc = R"glsl(
		#version 450 core
		out vec4 color;
		in VS_OUT
		{
			vec4 color;
		} fs_in;
		void main (void) {
			color = fs_in.color;
		}
	)glsl";
		program = glCreateProgram();
		compileShader(program, vShaderSrc, GL_VERTEX_SHADER);
		compileShader(program, fShaderSrc, GL_FRAGMENT_SHADER);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &compiled);
		GLchar Log[1024];
		glGetProgramInfoLog(program, 1024, NULL, Log);
		printf("\n%s\n", Log);
		glUseProgram(program);

		mLoc = glGetUniformLocation(program, "model_matrix");
		vLoc = glGetUniformLocation(program, "view_matrix");
		prjLoc = glGetUniformLocation(program, "proj_matrix");
		printf("\n%i, %i, %i \n", mLoc, vLoc, prjLoc);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLfloat vertex_data[] = {
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
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(vao);
		view_matrix = translate(0.0f, 0.0f, -20.0f);

		float aspect = (float)info.windowWidth / (float)info.windowHeight;
		proj_matrix = perspective(50.0f, aspect, 0.1f, 1000.0f);
	}
	void processInput(SDL_Event action) 
	{
		if (action.type == SDL_KEYDOWN)
		{
			if (action.key.keysym.sym == SDLK_w)vertical -= 1;
			if (action.key.keysym.sym == SDLK_s)vertical += 1;
			if (action.key.keysym.sym == SDLK_a)horizontal -= 1;
			if (action.key.keysym.sym == SDLK_d)horizontal += 1;
		}
	}
	void render(float currentTime) 
	{
		glClearColor(0.0, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glUniformMatrix4fv(prjLoc, 1, GL_FALSE, proj_matrix);

		for (int i = 0; i < 24; i++) {
			float f = currentTime * 0.03f;
			model_matrix = translate(0.0f, 0.0, 10.0f - (i)) *
				rotate(0.0f, 1.0f, 0.0f, currentTime * 4.5f) *
				rotate(1.0, 0.0, 0.0, currentTime * 2.1f) *
				rotate(0.0, 0.0, 1.0, currentTime * (i * 0.1))
				* translate(0.0f, sin(currentTime), 0.0f) *
				 translate(sinf(2.1 * f) * 2.0f,
					cosf(1.7f * f) * 2.0f,
					sinf(1.3 * f) * cosf(1.5 * f) * 2.0f); 
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, model_matrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		printf("%f, %f\r", horizontal, vertical);
		float xCoord = sin(horizontal);
		float zCoord = cos(horizontal);
		view_matrix = mat4_lookat(vec3(xCoord - 10, 0.0f, zCoord - 10), vec3(0.0, 0.0, 12.0), vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, view_matrix);
	}
protected:
	GLuint program;
	GLuint vao;
	GLuint buffer;
	GLint mLoc;
	GLint vLoc;
	GLint prjLoc; 
	GLint compiled;
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 proj_matrix;
	GLfloat horizontal;
	GLfloat vertical;
};

/*
int main(int argc, char** argv)
{
	flyingCubes* app = new flyingCubes();
	app->run(app);
	delete app;
	return 0;
} */