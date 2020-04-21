#define _USE_MATH_DEFINES 1;
#include "BaseLib.h"
#include <vmath/vmath.h>
#include "ShaderReader.h"

#define TORSO_HEIGHT 5.0f
#define TORSO_WIDTH 1.0f
#define UPPER_ARM_HEIGHT 3.0f
#define LOWER_ARM_HEIGHT 2.0f
#define UPPER_LEG_WIDTH  0.5f
#define LOWER_LEG_WIDTH  0.5f
#define LOWER_LEG_HEIGHT 2.0f
#define UPPER_LEG_HEIGHT 3.0f
#define UPPER_LEG_WIDTH  0.5f
#define UPPER_ARM_WIDTH  0.5f
#define LOWER_ARM_WIDTH  0.5f
#define HEAD_HEIGHT 1.5f
#define HEAD_WIDTH 1.0f

class matrix_stack
{
public:
	static const int MAX = 50;
	matrix_stack() { index = 0; }
	void push(const mat4& matrix)
	{
		if (index + 1 < MAX) {
			matrices[index++] = matrix;
		}
		else { return; }
	}
	mat4 pop()
	{
		index--;
		return matrices[index];
	}
private:
	mat4 matrices[MAX];
	int index;
};

class robot_tree
{

public:
	typedef struct node
	{
		mat4 m;
		void (robot_tree::* f)();
		struct node* sibling;
		struct node* child;
	}treenode;

	void initNodes()
	{
		mat4 m;

		m = rotate(theta[0], 0.0f, 1.0f, 0.0f);
		torso_node.f = &robot_tree::torso;
		torso_node.m = m;
		torso_node.child = &head_node;

		m = translate(0.0f, TORSO_HEIGHT + 0.5f * HEAD_HEIGHT, 0.0f) *
			rotate(theta[1], 1.0f, 0.0f, 0.0f) *
			rotate(theta[2], 0.0f, 1.0f, 0.0f);
		head_node.m = m;
		head_node.f = &robot_tree::head;
		head_node.sibling = &lua_node;

		m = translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH), 0.9f * TORSO_HEIGHT, 0.0f)
			* rotate(theta[3], 1.0f, 0.0f, 0.0f);
		lua_node.m = m;
		lua_node.f = &robot_tree::upper_arm;
		lua_node.sibling = &rua_node;
		lua_node.child = &lla_node;

		m = translate(0.0f, UPPER_ARM_HEIGHT, 0.0f)
			* rotate(theta[4], 1.0f, 0.0f, 0.0f);
		lla_node.m = m;
		lla_node.f = &robot_tree::lower_arm;

		m = translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9f * TORSO_HEIGHT, 0.0f)
			* rotate(theta[5], 1.0f, 0.0f, 0.0f);
		rua_node.m = m;
		rua_node.f = &robot_tree::upper_arm;
		rua_node.child = &rla_node;

		m = translate(0.0f, UPPER_ARM_HEIGHT, 0.0f)
			* rotate(theta[6], 1.0f, 0.0f, 0.0f);
		rla_node.m = m;
		rla_node.f = &robot_tree::lower_arm;

		m = translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH), 0.1f * UPPER_LEG_HEIGHT, 0.0f)
			* rotate(theta[7], 1.0f, 0.0f, 0.0f);
		lul_node.m = m;
		lul_node.f = &robot_tree::upper_leg;
		lul_node.sibling = &rul_node;
		lul_node.child = &lll_node;

		m = translate(0.0f, UPPER_LEG_HEIGHT, 0.0f)
			* rotate(theta[8], 1.0f, 0.0f, 0.0f);
		lll_node.m = m;
		lll_node.f = &robot_tree::lower_leg;

		m = translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH), 0.1f * UPPER_LEG_HEIGHT, 0.0f)
			* rotate(theta[9], 1.0f, 0.0f, 0.0f);
		rul_node.m = m;
		rul_node.f = &robot_tree::upper_leg;
		rul_node.child = &rll_node;

		m = translate(0.0f, UPPER_LEG_HEIGHT, 0.0f)
			* rotate(theta[10], 1.0f, 0.0f, 0.0f);
		rll_node.m = m;
		rll_node.f = &robot_tree::lower_leg;
	}
	void traverse(node* node)
	{
		if (node == NULL) return;
		mvstack.push(modelview);
		modelview *= node->m;
		node->f;
		if (node->child) traverse(node->child);
		if (node->sibling) traverse(node->sibling);
	}
	matrix_stack  mvstack;
	mat4 modelview;
	GLfloat theta[11];
	GLuint mvLoc;
	node  torso_node;
private:
	void torso()
	{
		mvstack.push(modelview);
		instance = translate(0.0f, 0.5f * TORSO_HEIGHT, 0.0f) *
			scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH);
		modelview *= instance;
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
		modelview = mvstack.pop();
		printf("Torso traversed!\n");
	}
	void head()
	{
		mvstack.push(modelview);
		instance = translate(0.0f, 0.5f * HEAD_HEIGHT, 0.0f) *
			scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH);
		modelview *= instance;
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
		modelview = mvstack.pop();
		printf("Head traversed!\n");
	}
	void upper_arm()
	{
		mvstack.push(modelview);
		instance = translate(0.0f, 0.5f * UPPER_ARM_HEIGHT, 0.0f) *
			scale(UPPER_ARM_WIDTH, UPPER_LEG_HEIGHT, UPPER_ARM_WIDTH);
		modelview *= instance;
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
		modelview = mvstack.pop();
		printf("UpperArm traversed!\n");
	}
	void upper_leg()
	{
		mvstack.push(modelview);
		instance = translate(0.0f, 0.5f * UPPER_LEG_HEIGHT, 0.0f) *
			scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH);
		modelview *= instance;
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
		modelview = mvstack.pop();
		printf("Upperleg traversed!\n");
	}
	void lower_arm()
	{
		mvstack.push(modelview);
		instance = translate(0.0f, 0.5f * LOWER_ARM_HEIGHT, 0.0f)
			* scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH);
		modelview *= instance;
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
		printf("Lower arm traversed!\n");
	}
	void lower_leg()
	{
		mvstack.push(modelview);
		instance = translate(0.0f, 0.5f * LOWER_LEG_HEIGHT, 0.0f)
			* scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH);
		modelview *= instance;
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
		printf("Lower leg traversed!\n");
	}
	mat4 instance;
	node head_node, lua_node, rua_node,
		lll_node, rll_node, lla_node,
		rla_node, rul_node, lul_node;
};

class robot_scene : public OGLApp
{
	robot_tree robot;
	matrix_stack mvstack;
	void startup()
	{
		ShaderInfo shaders[] =
		{
			{GL_VERTEX_SHADER, "robotVert2004.glsl"},
			{GL_FRAGMENT_SHADER, "robotFrag2004.glsl"},
			{GL_NONE, NULL}
		};
		program = glCreateProgram();
		program = LoadShaders(shaders);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glUseProgram(program);
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		static const GLfloat cube_pos[] =
		{
			-1.0f, -1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, -1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, -1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		static const GLushort cube_indices[] =
		{
			0, 1, 2, 3,
			6, 7, 4, 5,
			0xFFFF,
			2, 6, 0, 4,
			1, 5, 3, 7
		};
		glCreateBuffers(2, buffers);
		glNamedBufferStorage(buffers[0], sizeof(cube_pos), cube_pos, 0);
		glNamedBufferStorage(buffers[1], sizeof(cube_indices), cube_indices, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);


		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		locs[0] = glGetUniformLocation(program, "mv_matrix");
		locs[1] = glGetUniformLocation(program, "prj_matrix");

		prj_matrix = perspective(50.0f, (float)info.width / (float)info.height, 1.0, 100.0f);
		glUniformMatrix4fv(locs[1], 1, GL_FALSE, prj_matrix);
		mv_matrix = mat4_lookat(vec3(5.0f, 5.0f, 20.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(locs[0], 1, GL_FALSE, mv_matrix);
		glClearColor(0.0, 0.5, 0.5, 1.0);

		robot.mvstack = mvstack;
		robot.modelview = mv_matrix;
		robot.mvLoc = locs[0];

		robot.initNodes();

		robot.theta[0] = 0.0f;
		robot.theta[1] = 0.0f;
		robot.theta[2] = 0.0f;
		robot.theta[3] = 0.0f;
		robot.theta[4] = 0.0f;
		robot.theta[5] = 0.0f;
		robot.theta[6] = 0.0f;
		robot.theta[7] = 180.0f;
		robot.theta[8] = 0.0f;
		robot.theta[9] = 180.0f;
		robot.theta[10] = 0.0f;
	}
	void render(float currentTime)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		robot.traverse(&robot.torso_node);
	}
protected:
	GLuint program;
	GLuint vao;
	GLuint buffers[2];
	GLuint locs[2];
	mat4 mv_matrix;
	mat4 prj_matrix;
};

int main(int argc, char** argv)
{
	robot_scene* a = new robot_scene;
	a->run(a);
	delete a;
	return 0;
}