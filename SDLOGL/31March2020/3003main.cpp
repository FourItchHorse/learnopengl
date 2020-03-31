#include "OGLScene.h"
class redGreenBg : public OGLScene
{
public:
	void render(float currentTime)
	{
		static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, red);
	}
};
int main(int argc, char** argv)
{
	redGreenBg* app = new redGreenBg();
	app->run(app);
	delete app;
	return 0;
}