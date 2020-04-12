#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <GL/gl3w.h>
#include <GL/glut.h>
#include <vmath.h>
#include <stdio.h>
#include <chrono>
#include <math.h>
class OGLScene
{
   public:
	   OGLScene(){}
	   virtual ~OGLScene(){}
	   struct APPINFO
	   {

	   };
	   virtual void run()
	   {
		//init windowing system lib
	   	init(); //sets window attribs dependent on APPINFO
		//set OGL version
		//
		//create window


		if(gl3wInit){printf("GL3W failed to init! Error: %i", gl3wInit()); return}
		printf("GL_VERSION: %s\nGL_VENDOR: %s\nGL_RENDERER: %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));

		startup();

		do
		{
		while();
		
	   }
	  virtual void init()
	  {
	  }

	  void compileShader()
	  {
	  }

	  virtual void startup()
	  {
	  }

	  virtual void render()
	  {
	  }

	  virtual void shutdown()
	  {
	  }
	 
protected:
	  APPINFO info;
	  static  OGLScene * scene;


};
#define DECLARE_MAIN(a);
	  int main (int argc, char** argv)
	  {
	  	glutInit(&argc, argv);
		a  *scene = new a;
		scene->run();
		delete scene;
		return 0;
	  }
