#include "OGLScene.h"
class myCube : public OGLScene 
{
	void startup() 
	{
		const GLchar* vShaderSrc = R"glsl(
		#version 450 core
		layout (location = 0) in vec4 pos;
		layout (location = 1) in vec4 color;
		layout (location = 2) uniform mat4 proj;
		layout (location = 3) uniform mat4 view; 
        layout (location = 4) uniform mat4 model;
		out VS_OUT { vec4 color; } vs_out;
		void main () 
		{
			gl_Position = proj * view * model * pos;
			vs_out.color = color;
		}
		)glsl";
		const GLchar* fShaderSrc = R"glsl(
		#version 450 core
		in VS_OUT{ vec4 color; }fs_in;
		out vec4 outCol;
		void main () 
		{
			outCol = fs_in.color;
		}
		)glsl";
		program = glCreateProgram();
		OGLScene::compileShader(program, vShaderSrc, GL_VERTEX_SHADER);
		OGLScene::compileShader(program, fShaderSrc, GL_FRAGMENT_SHADER);
		glLinkProgram(program);
		glUseProgram(program);
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		static const GLfloat cube_corners[] = {
			-1.0f, -1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};
		static const GLfloat cube_colors[] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f
		};
		static GLushort cube_indices[] =
		{
			0, 1, 2, 3,6, 7, 4, 5,
			0xFFFF, 
			2, 6, 0, 4, 1, 5, 3, 7
		};
		glGenBuffers(2, buffers);
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_corners) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_corners), cube_corners);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_corners), sizeof(cube_colors), cube_colors);

		
		proj_matrix = perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
		glUniformMatrix4fv(2, 1, GL_FALSE, proj_matrix);

		
		view_matrix = mat4_lookat(vec3(5.0f, -10.0f, -50.0f), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(3, 1, GL_FALSE, view_matrix);

		model = MAT4_IDENTITY;
		glUniformMatrix4fv(4, 1, GL_FALSE, model);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *) sizeof(cube_corners));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	}
	void render(float currentTime)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	}
protected:
	GLuint buffers[2];
	GLuint program;
	GLuint vao;

	mat4 proj_matrix;
	mat4 view_matrix;
	mat4 model;
};


DECLARE_MAIN(myCube);
int main(int argc, char** argv)
{
	myCube* a = new myCube();
	a->run(a);
	delete a;
	return 0;
}