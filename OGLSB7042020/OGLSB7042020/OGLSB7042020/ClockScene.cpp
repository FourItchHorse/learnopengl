/*
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
		for (double i = 0; i <= (M_PI * 2); i += (M_PI/numSegments))
		{
			array[j++] = cos(i) * radius;
			array[j++] = sin(i) * radius;
			array[j++] = cos(i) * 0.5 + 0.5;
			array[j++] = sin(-i) * 0.5 + 0.5;
		}
	}
	void startup()
	{
		glCreateVertexArrays(NumVAOs, vaos);
		glBindVertexArray(vaos[0]);
		glCreateBuffers(NumBuffers, vbos);
		printf("Creating VBOS and VAOs: %i\n", glGetError());
		glBindVertexArray(vaos[0]);
		ShaderInfo faceShaders[]{
		{GL_VERTEX_SHADER, "FaceVShader.glsl" },
		{GL_FRAGMENT_SHADER, "FaceFShader.glsl"},
		{GL_NONE, NULL}};
		programs[faceProgram] = LoadShaders(faceShaders);
		printf("Creating face shader: %i\n", glGetError());
		GLfloat clock[52];
		circle(clock, 6, 1.0);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(clock), clock, GL_STATIC_DRAW);
		printf("Clock buffer object establishment: %i\n", glGetError());
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		printf("Clock vertex array establishment: %i\n", glGetError());

		unsigned char* FaceTexture;
		int width, height, nrChannels;
		glCreateTextures(GL_TEXTURE_2D,NumTextures, textures);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		FaceTexture = stbi_load("Debug/clockTex.png", &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, FaceTexture);
		stbi_image_free(FaceTexture);
		printf("Clock texture object establishment: %i\n", glGetError());


		glBindVertexArray(vaos[Hands]);
		ShaderInfo handShaders[]{
		{GL_VERTEX_SHADER, "HandsVShader.glsl" },
		{GL_FRAGMENT_SHADER, "HandsFShader.glsl"},
		{GL_NONE, NULL} };
		programs[handsProgram] = LoadShaders(handShaders);
		printf("Hands program establishment : %i", glGetError());
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
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(hand), hand, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribI1i(1, i);
			hand[3] *= i;
		}
		glUniform1f(3, M_PI/180.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void render(float currentTime)
	{
		GLfloat bgCol[] = {0.0, 0.5, 0.5, 1.0};
		glClearBufferfv(GL_COLOR, 0, bgCol);
		glBindVertexArray(vaos[0]);
		glUseProgram(programs[faceProgram]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 52);
		
		glBindVertexArray(vaos[Hands]);
		for (int i = 1; i < 3; i++)
		{
			glUseProgram(programs[handsProgram]);
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
*/