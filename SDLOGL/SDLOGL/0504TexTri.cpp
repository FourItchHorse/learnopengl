#include "OGLScene.h"

class TexTri : public OGLScene
{
	void startup() {
	static const GLchar* vertexShaderSource = R"glsl(
	#version 450 core
	layout (location = 0) in vec4 offset;
	layout (location = 1) in vec4 color;
	out VS_OUT
	{
		vec4 color;
	} vs_out;
	void main(void) 
	{
	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
				         vec4(-0.25, -0.25, 0.5, 1.0),
					 vec4(0.25, 0.25, 0.5, 1.0));
	gl_Position = vertices[gl_VertexID] + offset;
	vs_out.color = color;
	}
	)glsl";
	static const GLchar* fragmentShaderSource = R"glsl(
	#version 450 core
	out vec4 color;
	void main(void) 
	{
		color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,
		     cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,
		     sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15), 0.0f);
	}
	)glsl";
	program = glCreateProgram();
        compileShader(program, vertexShaderSource, GL_VERTEX_SHADER); 		
        compileShader(program, fragmentShaderSource, GL_FRAGMENT_SHADER);
	glLinkProgram(program);	
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glClearColor(0.0, 0.5, 0.5, 1.0);
	}
	void render(float currentTime) 
	{
		float redY = sin(currentTime) * 0.5f + 0.5f;
		float greenX = cos(currentTime) * 0.5f + 0.5f;
		const GLfloat color[] = {redY, greenX, 0.0, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);
		GLfloat attrib[] = {sin(currentTime) * 0.5f,
			    cos(currentTime) * 0.6f,
			    0.0f, 0.0f };
		glVertexAttrib4fv(0, attrib);
		glUseProgram(program);
		glDrawArrays(GL_PATCHES, 0, 3);

	}
	private:
	void generate_texture(float * data, int width, int height) 
	{
		int x, y;
		for(y=0;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				data[(y*width + x) * 4 + 0] = (float)((x&y) & 0xFF)/255.0f;
				data[(y*width + x) * 4 + 1] = (float)((x|y) & 0xFF)/255.0f;
				data[(y*width + x) * 4 + 2] = (float)((x&y) & 0xFF)/255.0f;
				data[(y*width + x) * 4 + 3] = 1.0f;
			}
		}
	}
	GLuint program;
	GLuint vao;
};
int main (int argc, char** argv)
{
	TexTri * a  = new TexTri();
	a->run(a);
	delete a;
	return 0;
}
