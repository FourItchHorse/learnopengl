#define _CRT_SECURE_NO_WARNINGS 1
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#undef main 
#include <SDL2/SDL_opengl.h>

#include <chrono>
#include <stdio.h>
#include <string.h>
#include <math.h>

	class OGLApp
	{
		public:
			OGLApp() {}
			virtual ~OGLApp() {}
			virtual void run (OGLApp* my_app)
			{
				bool running = true;
				app = my_app;
				if(SDL_Init(SDL_INIT_EVERYTHING))
				{
					printf("Failed to init SDL2!\n"); 
					return;
				}
				init();

				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, info.majorVer);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, info.minorVer);

				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			
				 if( info.flags.debug)
				 {
					 SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
				 }
				 SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, info.stencils);

				 window = SDL_CreateWindow(info.title, info.xPos, info.yPos, 
						 info.width, info.height, SDL_WINDOW_OPENGL);
				 if(!window)
				 {
					 printf("Failed to create window!\n"); 
					 return;
				 }
				 SDL_GL_CreateContext(window);

				 if(gl3wInit()) 
				 {
				 	printf("Unable to init GL3W! %i\n", gl3wInit());
					return;
				 }

				if(info.flags.debug) 
				{
				printf("GL_VERSION: %s\nGL_VENDOR: %s\nGL_RENDERER: %s\n", 
				glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));
				}

				startup();
				
				auto t_start = std::chrono::high_resolution_clock::now();
				SDL_Event event;

				do 
				{
					auto t_now = std::chrono::high_resolution_clock::now();
					float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
					render(time);
					SDL_GL_SwapWindow(window);
					if(SDL_PollEvent(&event))
					{
						processInput(event);
						if (event.type == SDL_QUIT) break; 
						if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) break;
					}
				} while(running);

				shutdown();

				SDL_DestroyWindow(window);
				SDL_Quit();

			}

			virtual void init()
			{
				strcpy(info.title, "OGLSB7 practice 042020");
				info.width = 800;
				info.height = 600;
				info.xPos = SDL_WINDOWPOS_CENTERED;
				info.yPos = SDL_WINDOWPOS_CENTERED;
				info.majorVer = 4;
				info.minorVer = 3;
				info.stencils = 0;
				info.flags.all = 0;
				info.flags.debug = 0;
			}

			virtual void processInput(SDL_Event action){}
			virtual void startup() {}
			virtual void render(float currentTime) {}
			virtual void shutdown(){}

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
				union
				{
					struct
					{
						unsigned int fullscreen  : 1;
						unsigned int vsync : 1;
						unsigned int debug : 1;
					};
					unsigned int all;
				}flags;
			};

	protected:
		APPINFO info;
		SDL_Window* window;
		OGLApp* app;
};

