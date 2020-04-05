
#include "OGLScene.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class TexTri : public OGLScene
{
	public:
	GLuint myProgram;

	void startup()
	{
	const GLchar * vShaderSrc = R"glsl(
	#version 450 core
	layout (location = 0) in vec3 inPos;
	void main (void)
	{
		gl_Position = vec4(inPos, 1.0);
	}
	)glsl";
	const  GLchar * fShaderSrc = R"glsl(
	#version 450 core
	uniform sampler2D s;
	out vec4 color;
	void main (void)
	{
		color = texelFetch(s, ivec2(gl_FragCoord.xy), 0);
	}
	)glsl";	


	GLfloat vertices[] = {
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,
	 0.5, 0.5, 0.5,
	};
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);
	float * data = new float[256 * 256 * 4];
	generate_texture(data, 256, 256);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256,256, GL_RGBA, GL_FLOAT, data);
	delete [] data;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(0);

	program = glCreateProgram();
	compileShader(program, fShaderSrc, GL_FRAGMENT_SHADER);
	compileShader(program, vShaderSrc, GL_VERTEX_SHADER);
	glLinkProgram(program);
	glUseProgram(program);

	glClearColor(0.0, 0.5, 0.5, 1.0);
	}
	void render(float currentTime) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
protected:
	GLuint buffer;
	GLuint vao;
	GLuint texture;
	GLuint program;
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
};
int main (int argc, char** argv)
{
	TexTri* a = new TexTri();
	a->run(a);
	delete a;
}
