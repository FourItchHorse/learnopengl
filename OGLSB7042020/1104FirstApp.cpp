#include "SDLScene.h"

class my_first_app : public OGLApp
{
	public:
		void render(float currentTime)
		{
			glClearColor ((float)sin(currentTime) * 0.5f + 0.5f,
				      (float)cos(currentTime) * 0.5f + 0.5f, 
						               0.0f,  1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
};
/*
int main (int argc, char** argv)
{
	my_first_app *scene = new my_first_app();
	scene->run(scene);
	delete scene;
	return 0;
} */

