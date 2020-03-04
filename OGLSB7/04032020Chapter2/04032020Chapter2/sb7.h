#pragma once
#ifndef __SB7_H__
#define __SB7_H__

#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

namespace sb7 {


	class application
	{
	private: 
		static void APIENTRY debug_callback(GLenum source,
											GLenum type,
											GLuint id,
											GLenum severity,
											GLsizei
											);
	};
}
#endif
