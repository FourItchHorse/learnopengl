#define _USE_MATH_DEFINES 1;
#include "BaseLib.h"
#include <vmath.h>
#include "ShaderReader.h"

class robot_tree
{

};
class robot_scene : public OGLApp
{
	void startup()
	{
		ShaderInfo shaders[] =
		{
			{GL_VERTEX_SHADER, "robotVert2004.glsl"},
			{GL_FRAGMENT_SHADER, "robotFrag2004.glsl"},
			{GL_NONE, NULL}
		};
		program = glCreateProgram();
		program = LoadShaders(shaders);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glUseProgram(program);
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		static const GLfloat cube_pos[] = 
		{
			-1.0f, -1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, -1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, -1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		static const GLushort cube_indices[] = 
		{
			0, 1, 2, 3, 
			6, 7, 4, 5, 
			0xFFFF, 
		    2, 6, 0, 4,
			1, 5, 3, 7
		};
		glCreateBuffers(2, buffers);
		glNamedBufferStorage(buffers[0], sizeof(cube_pos), cube_pos, 0);
		glNamedBufferStorage(buffers[1], sizeof(cube_indices), cube_indices, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);


		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) 0);
		glEnableVertexAttribArray(0);

		locs[0] = glGetUniformLocation(program, "mv_matrix");
		locs[1] = glGetUniformLocation(program, "prj_matrix");

		
		prj_matrix = vmath::perspective(50.0f, (float)info.width/(float)info.height, 1.0, 100.0f);
		glUniformMatrix4fv(locs[1], 1, GL_FALSE, prj_matrix);
		mv_matrix = vmath::lookat(vmath::vec3(5.0f, 5.0f, 20.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(locs[0], 1, GL_FALSE, mv_matrix);
		glClearColor(0.0, 0.5, 0.5, 1.0);

	}
	void render(float currentTime)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glUseProgram(program);
		mv_matrix *= vmath::rotate(0.1f, 1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(locs[0], 1, GL_FALSE, mv_matrix);
		glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_SHORT, NULL);
	}
protected:
	GLuint program;
	GLuint vao;
	GLuint buffers[2];
	GLuint locs[2];
	vmath::mat4 mv_matrix;
	vmath::mat4 prj_matrix;
};

int main(int argc, char** argv)
{
	robot_scene *a = new robot_scene;
	a->run(a);
	delete a;
	return 0;
}