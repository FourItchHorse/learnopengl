
#include <vmath/vmath.h>

int main (int argc, char** argv) 
{
 vmath::vec3 triangle[] = {vmath::vec3(-0.5f, 0.0f, 0.0f),
 			   vmath::vec3(0.5f, 0.0f, 0.0f),
			   vmath::vec3(0.0f, 0.5f, 0.0f) }; //stores a triangle akin to the one drawn in the 3rd chapter
 vmath::vec4 transformableTriangle[] = {vmath::vec4(-0.5f, 0.0f, 0.0f, 1.0f),
	 				vmath::vec4(0.5f, 0.0f, 0.0f, 1.0f),
					vmath::vec4(0.0f, 0.5f, 0.0f, 1.0f)}; //stores a triangle ready to be multiplied by a 4x4 transformation matrix: note that it makes the w column into an identity matrix(????) 

}
