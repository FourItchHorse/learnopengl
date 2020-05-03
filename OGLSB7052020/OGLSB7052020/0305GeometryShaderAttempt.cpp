
#include "SDLBase.h"
#include "ShaderReader.h"

//GOAL: make a geometry shader that takes a point 
//and outputs a cube and create a program to view 
//said cube in 3D visa vi blender's numpad system
class ThirdMayTwentyTwenty : public SDLOGL
{
	void init()
	{
		
		glCreateVertexArrays(1, &vao);
		glCreateBuffers(1, &vbo);
		glBindVertexArray(vao);
		GLfloat pointCoord[16] = { 0.0f, 0.0f, 0.0f, 1.0f,
								  0.0f, 3.0f, 0.0f, 1.0f,
								  0.0f, 0.0f, 3.0f, 1.0f, 
								  3.0f, 0.0f, 0.0f, 1.0f};
		glNamedBufferStorage(vbo, sizeof(pointCoord), pointCoord, 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		ShaderInfo cubeShaders[]
		{
			{GL_VERTEX_SHADER, "thirdMayVertex.glsl"},
			{GL_GEOMETRY_SHADER, "thirdMayGeometry.glsl"},
			{GL_FRAGMENT_SHADER, "thirdMayFragment.glsl"},
			{GL_NONE, NULL}
		};
		program = LoadShaders(cubeShaders);
		glUseProgram(program);

		proj_matrix = vmath::perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
		view_matrix = vmath::lookat(vmath::vec3(-2.0f, -5.0f, -2.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
		model_matrix = vmath::mat4::identity();

		mvp_matrix = proj_matrix * view_matrix * model_matrix;
		glUniformMatrix4fv(1, 1, GL_FALSE, mvp_matrix);

		glBindVertexArray(vao);
		glUseProgram(program);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.5, 0.5, 1.0);
	}
	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mvp_matrix = proj_matrix * view_matrix * model_matrix;
		model_matrix *= vmath::rotate(0.05f, vmath::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(1, 1, GL_FALSE, mvp_matrix);
		glDrawArrays(GL_POINTS, 0, 4);
	}
protected:
	GLuint vao, vbo, program;
	vmath::mat4 model_matrix, view_matrix, proj_matrix, mvp_matrix;
};

int main(int argc, char** argv)
{
	ThirdMayTwentyTwenty* a = new ThirdMayTwentyTwenty;
	a->run();
	delete a;
	return 0;
} 
//*/