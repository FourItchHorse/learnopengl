/*
#define _USE_MATH_DEFINES 1
#include  "BaseLib.h"
#include "ShaderReader.h"
#include <vmath.h>

#define TORSO_HEIGHT 5.0
#define TORSO_WIDTH 1.0
#define UPPER_ARM_HEIGHT 3.0
#define LOWER_ARM_HEIGHT 2.0
#define UPPER_LEG_WIDTH  0.5
#define LOWER_LEG_WIDTH  0.5
#define LOWER_LEG_HEIGHT 2.0
#define UPPER_LEG_HEIGHT 3.0
#define UPPER_LEG_WIDTH  0.5
#define UPPER_ARM_WIDTH  0.5
#define LOWER_ARM_WIDTH  0.5
#define HEAD_HEIGHT 1.5
#define HEAD_WIDTH 1.0

class matrix_stack
{
public:
	static const int MAX = 60;
	matrix_stack() { index = 0; }
	void push(const vmath::mat4& matrix) 
	{
		matrices[index] = matrix;
		index++;
	}
	  vmath::mat4 pop() 
	{
		index--;
		return matrices[index];
	}
private:
	vmath::mat4 matrices[MAX];
	int index;
};


vmath::mat4 model_view;
matrix_stack mvstack;
GLuint mvLoc;

void torso()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * TORSO_HEIGHT, 0.0) *  vmath::scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void head()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * HEAD_HEIGHT, 0.0) *  vmath::scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void left_upper_arm()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *  vmath::scale(UPPER_ARM_WIDTH, UPPER_LEG_HEIGHT, UPPER_ARM_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void left_lower_arm()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *  vmath::scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void right_upper_arm()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *  vmath::scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void right_lower_arm()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *  vmath::scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view * &instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void left_upper_leg()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *  vmath::scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void left_lower_leg()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *  vmath::scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH);
	model_view *= instance;
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view * instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void right_upper_leg()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *  vmath::scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH);
	model_view *= instance;
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}
void right_lower_leg()
{
	mvstack.push(model_view);
	 vmath::mat4 instance =  vmath::translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *  vmath::scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH);
	model_view *= instance;
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_BYTE, NULL);
	model_view = mvstack.pop();
}

class myRobot : public OGLApp {
	typedef struct treenode
	{
		 vmath::mat4 m;
		void (*f)(void);
		struct treenode *sibling;
		struct treenode *child;

		treenode() : f(NULL), sibling(NULL), child(NULL) {}
	} treenode;
	treenode torso_node, head_node, lua_node, rua_node, lll_node,
			    rll_node, lla_node, rla_node, rul_node, lul_node;
	
	void traverse(treenode* root)
	{
		if (root == NULL)return;
		mvstack.push(model_view);
		model_view = model_view * root->m;
		root->f;
		if (root->child != NULL)traverse(root->child);
		model_view = mvstack.pop();
		if (root->sibling != NULL)traverse(root->sibling);
	}
	
	void initNodes()
	{
		 vmath::mat4 m;

		m = rotate(0.0, 1.0, 0.0, theta[0]); 
		torso_node.m = m;
		torso_node.f = torso;
		torso_node.child = &head_node;

		m =  vmath::translate(0.0, TORSO_HEIGHT+0.5*HEAD_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[1]) * rotate(0.0, 1.0, 0.0, theta[2]);
		head_node.m = m;
		head_node.f = &head;
		head_node.sibling = &lua_node;

		m =  vmath::translate(-(TORSO_WIDTH+UPPER_ARM_WIDTH), 0.9*TORSO_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[3]);
		lua_node.m = m;
		lua_node.f = left_upper_arm;
		lua_node.child = &lla_node;
		lua_node.sibling = &rua_node;
		
		m =  vmath::translate((TORSO_WIDTH + UPPER_ARM_WIDTH), 0.9 * TORSO_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[5]);
		rua_node.m = m;
		rua_node.f = &right_upper_arm;
		rua_node.child = &rla_node;
		rua_node.sibling = &lua_node;

		m =  vmath::translate(-(TORSO_WIDTH+UPPER_LEG_WIDTH), 0.1*UPPER_LEG_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[7]);
		lul_node.m = m;
		lul_node.f = &left_upper_leg;
		lul_node.child = &lll_node;
		lul_node.sibling = &rul_node;

		m =  vmath::translate((TORSO_WIDTH + UPPER_LEG_WIDTH), 0.1 * UPPER_LEG_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[9]);
		rul_node.m = m;
		rul_node.f = right_upper_leg;

		m =  vmath::translate(0.0, UPPER_ARM_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[4]);
		lua_node.m  = m;
		lua_node.f = left_lower_arm;

		m =  vmath::translate(0.0, UPPER_ARM_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[5]);
		rua_node.m = m;
		rua_node.f = right_lower_arm;

		m =  vmath::translate(0.0, UPPER_LEG_HEIGHT, 0.0)*rotate(1.0, 0.0, 0.0, theta[6]);
		lll_node.m = m;
		lll_node.f = left_lower_leg;

		m = (0.0, UPPER_LEG_HEIGHT, 0.0) * rotate(1.0, 0.0, 0.0, theta[6]);
		rll_node.m = m;
		rll_node.f = right_lower_leg;

	}
	void startup() 
	{
		ShaderInfo shaders[]{
		{GL_VERTEX_SHADER, "1804RobotVertex.glsl"},
		{GL_FRAGMENT_SHADER, "1804RobotFragment.glsl"},
		{GL_NONE, NULL}
		};
		program = glCreateProgram();
		program = LoadShaders(shaders);
		glCreateVertexArrays(NumVAOs, vaos);
		glCreateBuffers(NumBuffers, buffers);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		const GLfloat cubeVerts[] = {-1.0f, -1.0f, -1.0f, 
									 -1.0f, -1.0f,  1.0f,
									 -1.0f, 1.0f,  -1.0f,
									 -1.0f, 1.0f,   1.0f,
									};
		const GLubyte cubeInds[] = {0, 1, 2, 3, 6, 7, 4, 5,
									0xFFFF, 
									2, 6, 0, 4, 1, 5, 3, 7};
		glCreateBuffers(NumBuffers, buffers);
		glNamedBufferStorage(buffers[ArrayBuffer], sizeof(cubeVerts), cubeVerts, 0);
		glNamedBufferStorage(buffers[IndexBuffer], sizeof(cubeInds), cubeInds, 0);
	
		
		glBindVertexArray(vaos[Cube]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[ArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);

		initNodes();
		glClearColor(0.0, 0.5, 0.5, 1.0);
	}
	

	void render(float currentTime) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		model_view = mat4_lookat(vec3(10.0, 5.0, -10.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
		traverse(&torso_node);
	}
protected:
	enum Buffer_IDs {ArrayBuffer, IndexBuffer, NumBuffers};
	enum VAO_IDs {Cube, NumVAOs};
	GLuint program;
	GLuint buffers[NumBuffers];
	GLuint vaos[NumVAOs];
	GLint theta[10];
};

int main(int argc, char** argv)
{
	myRobot* scene = new myRobot();
	scene->run(scene);
	delete scene;
	return 0;
} */