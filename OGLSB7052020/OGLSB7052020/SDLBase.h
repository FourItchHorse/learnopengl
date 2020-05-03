#pragma once 
#define _CRT_SECURE_NO_WARNINGS 1
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_opengl.h>
#include <vmath.h>

#include <stdio.h>

class SDLOGL
{
public:
	SDLOGL() {}
	virtual ~SDLOGL() {}
	virtual void run()
	{
		bool running = true;
		app = this;
		if (SDL_Init(SDL_INIT_EVERYTHING))
		{
			printf("Failed to init SDL2!\n");
			return;
		}
		setContextAttrs();
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, info.majorVer);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, info.minorVer);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, info.stencils);

		window = SDL_CreateWindow(info.title, info.xPos, info.yPos, info.width, info.height, SDL_WINDOW_OPENGL);
		if (!window)
		{
			printf("Failed to create window!\n");
			return;
		}
		SDL_GL_CreateContext(window);

		if (gl3wInit())
		{
			printf("Unable to initialise GL3W: %i!\n", gl3wInit());
			return;
		}

		printf("GL_VERSION: %s\nGL_VENDOR: %s\nGL_RENDERER: %s\n",
		glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));
		
		init();

		SDL_Event event;
		do
		{
			display();
			SDL_GL_SwapWindow(window);
			if (SDL_PollEvent(&event)) {
				processInput(event);
				running &= event.type != SDL_QUIT;
			}
		} while (running);

		shutdown();

		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	virtual void setContextAttrs()
	{
		strcpy(info.title, "OGL practice 052020");
		info.width = 800;
		info.height = 600;
		info.xPos = SDL_WINDOWPOS_CENTERED;
		info.yPos = SDL_WINDOWPOS_CENTERED;
		info.majorVer = 4;
		info.minorVer = 5;
		info.stencils = 8;
	}

	virtual void processInput(SDL_Event action) {

	}

	virtual void init() {}
	virtual void display() {}
	virtual void shutdown() {}

	struct APPINFO
	{
		char title[128];
		int width;
		int height;
		int xPos;
		int yPos;
		int majorVer;
		int minorVer;
		int stencils;
	};
protected:
	APPINFO info;
	SDL_Window* window;
	SDLOGL* app;
};
