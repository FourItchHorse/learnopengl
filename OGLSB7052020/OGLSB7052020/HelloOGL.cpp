
#include "SDLBase.h"
#include "ShaderReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES 1
#include <math.h>
#include <chrono>


enum VBOandVAO_IDs {Face, Sec, Min, Hour, NumVAOs, NumVBOs = NumVAOs};
enum Tex_IDs {FaceTexture = Face, NumTextures};
enum Program_IDs {FaceProgram = Face, Hands, NumPrograms};

class HelloOGL : public SDLOGL {
public:
	void circle(GLfloat array[], int numSegments, float radius)
	{
		array[0] = array[1] = 0.0f;
		array[2] = array[3] = 0.5f;
		int index = 4;
		double twoPi = 2 * M_PI;
		for (double i = 0; i <= twoPi; i += (twoPi /(double)numSegments))
		{
			array[index++] = -cosf(i) * radius;
			array[index++] = -sinf(i) * radius;
			array[index++] = -cosf(i) * 0.5f + 0.5f;
			array[index++] = (sinf(i) * 0.5f + 0.5f);
		}
	}
	void init()
	{
		glCreateVertexArrays(NumVAOs, vaos);
		ShaderInfo faceShaders[] = 
		{
			{GL_VERTEX_SHADER, "faceVert.glsl"},
			{GL_FRAGMENT_SHADER, "faceFrag.glsl"},
			{GL_NONE, NULL},
		};
		programs[Face] = LoadShaders(faceShaders);
		glUseProgram(programs[Face]);
		glCreateBuffers(NumVBOs, vbos);
		GLfloat vertices[52]; 
		circle(vertices, 12, 1.0f);
		glNamedBufferStorage(vbos[Face], sizeof(vertices), vertices, 0);

		glCreateTextures(GL_TEXTURE_2D, NumTextures, textures);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		int width, height, nrChannels;
		unsigned char* data = stbi_load("clockTex.png", &width, &height, &nrChannels, 0);
		glTextureSubImage2D(textures[0], 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, data);
		glGenerateTextureMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		glBindVertexArray(vaos[Face]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[Face]);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 52);
	}
protected:
	GLuint vaos[NumVAOs];
	GLuint vbos[NumVBOs];
	GLuint textures[NumTextures];
	GLuint programs[NumPrograms];
	const static int numVerts;
	float time;
};
int main(int argc, char** argv)
{
	HelloOGL *a = new HelloOGL;
	a->run();
	delete a;
	return 0;
}