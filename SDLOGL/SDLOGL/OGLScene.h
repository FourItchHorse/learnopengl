//framework based on OGLSB7 guide built on 30-31 March 2020
#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#undef main
#include <vmath/vmath.h>
#include <chrono>
#include <math.h>
class OGLScene
{
public:
	/*enum VAO_IDs { Triangles, NumVAOs };
	enum Buffer_IDs { ArrayBuffer, ElementBuffer, NumBuffers };
	enum Program_IDs { LocationBased, NumPrograms }; */

	OGLScene() {}
	virtual ~OGLScene() {}
	struct APPINFO
	{
		SDL_GLprofile profileVersion;
		char title[128];
		int windowX;
		int windowY;
		int windowWidth;
		int windowHeight;
		int majorVer;
		int minorVer;
		int samples;
	};

	virtual void run(OGLScene* my_scene)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING)) { printf("Unable to init SDL: %s\n", SDL_GetError()); return; }

		init();

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, info.majorVer);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, info.minorVer);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, info.profileVersion);

		window = SDL_CreateWindow(info.title, info.windowX, info.windowY, info.windowWidth, info.windowHeight, SDL_WINDOW_OPENGL);

		if (!window)
		{
			fprintf(stderr, "Failed to open window!\n");
			return;
		}

		context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, context);

		if (gl3wInit()) { printf("GL3W failed to init! Error: %i\n", gl3wInit()); return; }
		printf("GL_VERSION: %s\nGL_VENDOR %s\nGL_RENDERER: %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));

		startup();

		auto t_start = std::chrono::high_resolution_clock::now();
		SDL_Event event;

		GLboolean running = true;
		do
		{
			auto t_now = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
			render(time);
			SDL_GL_SwapWindow(window);
			if (SDL_PollEvent(&event)) {
				processInput(event);
				if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) break;
				if (event.type == SDL_QUIT) break;
			}

			printf("%f\r", time);

		} while (running);

		shutdown();
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	virtual void init()
	{

		info.majorVer = 4;
		info.minorVer = 3;
		info.profileVersion = SDL_GL_CONTEXT_PROFILE_CORE;
		strcat(info.title, "SDL OpenGL practice");
		info.windowX = SDL_WINDOWPOS_CENTERED;
		info.windowY = SDL_WINDOWPOS_CENTERED;
		info.windowWidth = 800;
		info.windowHeight = 600;


	}

	void compileShader(GLuint program, const GLchar* source, GLenum type) {
		GLuint s = glCreateShader(type);
		GLint compiled;
		glShaderSource(s, 1, &source, NULL);
		glCompileShader(s);
		glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLchar log[1024];
			glGetShaderInfoLog(s, 1024, NULL, log);
		}
		glAttachShader(program, s);
		glDeleteShader(s);
	}

	virtual void processInput(SDL_Event event)
	{

	}

	virtual void startup()
	{

	}

	virtual void render(float currentTime)
	{

	}

	virtual void shutdown()
	{

	}

#define DECLARE_MAIN(OGLScene);
	int main(int argc, char** argv)
	{
		OGLScene* app = new OGLScene();
		app->run(app);
		delete app;
		return 0;
	}

protected:
	APPINFO     info;
	static      OGLScene* my_scene;
	SDL_Window* window;
	SDL_GLContext context;

};

