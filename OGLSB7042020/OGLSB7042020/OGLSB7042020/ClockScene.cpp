
#define _USE_MATH_DEFINES 1
#include "BaseLib.h"
#include "ShaderReader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class clockScene : public OGLApp {
public:
	void circle(GLfloat * array, int numSegments, float radius)
	{
		array[0] = array[1] = 0.0;
		array[2] = array[3] = 0.5;
		int j = 4;
		for (double i = 0.1; i < M_2_PI; i += M_2_PI / numSegments)
		{
			array[j++] = cosf(i * radius);
			array[j++] = sinf(i * radius);
			array[j++] = cosf(i * 0.5 + 0.5);
			array[j++] = sinf(-i * 0.5 + 0.5);
		}
	}
	void startup()
	{
		glCreateVertexArrays(NumVAOs, vaos);
		glBindVertexArray(vaos[0]);
		glCreateBuffers(NumBuffers, vbos);
		
		glBindVertexArray(vaos[0]);
		ShaderInfo FaceShaders[]{
		{GL_VERTEX_SHADER, "FaceVShader.glsl" },
		{GL_FRAGMENT_SHADER, "FaceFShader.glsl"},
		{GL_NONE, NULL}};
		programs[faceProgram] = LoadShaders(FaceShaders);
		GLfloat clock0[28];
		circle(clock0, 6, 2.0);
		glNamedBufferStorage(vbos[0], sizeof(clock0), clock0, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		unsigned char* FaceTexture;
		int width, height, nrChannels;
		glCreateTextures(GL_TEXTURE_2D,NumTextures, textures);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		FaceTexture = stbi_load("clockTex.png", &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, FaceTexture);
		stbi_image_free(FaceTexture);

		glBindVertexArray(vaos[Hands]);
		ShaderInfo handShaders[]{
		{GL_VERTEX_SHADER, "HandsVShader.glsl" },
		{GL_FRAGMENT_SHADER, "HandsFShader.glsl"},
		{GL_NONE, NULL} };
		programs[handsProgram] = LoadShaders(handShaders);
		GLfloat hand[]
		{
			0.0, -0.1, 0.5,
		    1.5,  0.0, 0.5,
			0.0,  0.1, 0.5,
		};
		for (int i = 1; i < 3; i++) 
		{
			hand[3] /= i;
			hand[2] = hand[5] = hand[8] += 0.25;
			glNamedBufferStorage(vbos[i], sizeof(hand), &hand, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttrib1f(1, i);
			hand[3] *= i;
		}
		glUniform1f(3, M_PI/180.0f);
	}

	void render(float currentTime)
	{
		GLfloat bgCol[] = {0.0, 0.5, 0.5, 1.0};
		glClearBufferfv(GL_COLOR, 0, bgCol);
		glBindVertexArray(vaos[0]);
		glUseProgram(programs[faceProgram]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 28);
		glBindVertexArray(vaos[Hands]);
		glUseProgram(programs[handsProgram]);
		for (size_t i = 1; i < 3; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
protected:
	enum VAO_IDs {Hello, Hands, NumVAOs};
	enum BufferIDs {myBaby, Sec, Min, Hour, NumBuffers};
	enum Texture_IDs {helloMyHoney, NumTextures};
	enum Program_IDs {faceProgram, handsProgram, NumPrograms};
	GLuint vaos[NumVAOs];
	GLuint vbos[NumBuffers];
	GLuint textures[NumTextures];
	GLuint programs[NumPrograms];
}; 

int main(int argc, char** argv)                  
{                                                   
clockScene* app = new clockScene;                                 
app->run(app);                                  
delete app;                                     
return 0;                                       
}
