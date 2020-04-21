
#include "BaseLib.h"
#include "ShaderReader.h"
#include <vmath.h>
#include <assert.h>

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
#define HEAD_WIDTH 1.0f
#define HEAD_HEIGHT 1.5f

class matrix_stack
{
public:
	static const int MAX = 75;
	matrix_stack() { index = 0; }
	void push(const vmath::mat4& matrix)
	{
		if(index + 1  < MAX)
		matrices[index++] = matrix;
	}
	vmath::mat4 pop()
	{
		if(index -1 > 0)
		return matrices[index--];
	}
private:
	vmath::mat4 matrices[MAX];
	int index;
};

matrix_stack  mvstack;
vmath::mat4 modelview;
GLfloat theta[11];
GLuint mvLoc;
vmath::mat4 instance;


void torso()
{
	mvstack.push(modelview);
	instance = vmath::translate(0.0f, 0.5f * TORSO_HEIGHT, 0.0f) *
		vmath::scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview * instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	modelview = mvstack.pop();
	printf("A ");
}
void head()
{
	mvstack.push(modelview);
	instance = vmath::translate(0.0f, 0.5f * HEAD_HEIGHT, 0.0f) *
		vmath::scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview * instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	modelview = mvstack.pop();
	printf("B ");
}
void upper_arm()
{
	mvstack.push(modelview);
	instance = vmath::translate(0.0f, 0.5f * UPPER_ARM_HEIGHT, 0.0f) *
		vmath::scale(UPPER_ARM_WIDTH, UPPER_LEG_HEIGHT, UPPER_ARM_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview * instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	modelview = mvstack.pop();
	printf("C ");
}
void upper_leg()
{
	mvstack.push(modelview);
	instance = vmath::translate(0.0f, 0.5f * UPPER_LEG_HEIGHT, 0.0f) *
		vmath::scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview * instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	modelview = mvstack.pop();
	printf("D ");
}
void lower_arm()
{
	mvstack.push(modelview);
	instance = vmath::translate(0.0f, 0.5f * LOWER_ARM_HEIGHT, 0.0f)
		* vmath::scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview * instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	modelview = mvstack.pop();
	printf("E ");
}
void lower_leg()
{
	mvstack.push(modelview);
	instance = vmath::translate(0.0f, 0.5f * LOWER_LEG_HEIGHT, 0.0f)
		* vmath::scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, modelview * instance);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	modelview = mvstack.pop();
	printf("F \n");
}



class robot_scene : public OGLApp
{
public:
	typedef struct node
	{
		vmath::mat4 m;
		void (* f)(void);
		struct node* sibling;
		struct node* child;
	}treenode;
	

	void initNodes()
	{
		vmath::mat4 m;

		m = vmath::rotate(theta[0], 0.0f, 1.0f, 0.0f);
		torso_node.f = &torso;
		torso_node.m = m;
		torso_node.child = &head_node;

		m = vmath::translate(0.0f, TORSO_HEIGHT + 0.5f * HEAD_HEIGHT, 0.0f) *
			vmath::rotate(theta[1], 1.0f, 0.0f, 0.0f) *
			vmath::rotate(theta[2], 0.0f, 1.0f, 0.0f);
		head_node.m = m;
		head_node.f = &head;
		head_node.sibling = &lua_node;

		m = vmath::translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH), 0.9f * TORSO_HEIGHT, 0.0f)
			* vmath::rotate(theta[3], 1.0f, 0.0f, 0.0f);
		lua_node.m = m;
		lua_node.f = &upper_arm;
		lua_node.sibling = &rua_node;
		lua_node.child = &lla_node;

		m = vmath::translate(0.0f, UPPER_ARM_HEIGHT, 0.0f)
			* vmath::rotate(theta[4], 1.0f, 0.0f, 0.0f);
		lla_node.m = m;
		lla_node.f = &lower_arm;

		m = vmath::translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9f * TORSO_HEIGHT, 0.0f)
			* vmath::rotate(theta[5], 1.0f, 0.0f, 0.0f);
		rua_node.m = m;
		rua_node.f = &upper_arm;
		rua_node.child = &rla_node;

		m = vmath::translate(0.0f, UPPER_ARM_HEIGHT, 0.0f)
			* vmath::rotate(theta[6], 1.0f, 0.0f, 0.0f);
		rla_node.m = m;
		rla_node.f = &lower_arm;

		m = vmath::translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH), 0.1f * UPPER_LEG_HEIGHT, 0.0f)
			* vmath::rotate(theta[7], 1.0f, 0.0f, 0.0f);
		lul_node.m = m;
		lul_node.f = &upper_leg;
		lul_node.sibling = &rul_node;
		lul_node.child = &lll_node;

		m = vmath::translate(0.0f, UPPER_LEG_HEIGHT, 0.0f)
			* vmath::rotate(theta[8], 1.0f, 0.0f, 0.0f);
		lll_node.m = m;
		lll_node.f = &lower_leg;

		m = vmath::translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH), 0.1f * UPPER_LEG_HEIGHT, 0.0f)
			* vmath::rotate(theta[9], 1.0f, 0.0f, 0.0f);
		rul_node.m = m;
		rul_node.f = &upper_leg;
		rul_node.child = &rll_node;

		m = vmath::translate(0.0f, UPPER_LEG_HEIGHT, 0.0f)
			* vmath::rotate(theta[10], 1.0f, 0.0f, 0.0f);
		rll_node.m = m;
		rll_node.f = &lower_leg;
	}
	void traverse(node* node)
	{
		if (node == NULL) return;
		mvstack.push(modelview);
		modelview *= node->m;
		node->f();
		if (node->child) traverse(node->child);
		modelview = mvstack.pop();
		if (node->sibling) traverse(node->sibling);
		modelview = mvstack.pop();
	}
	
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

		prj_matrix = vmath::perspective(50.0f, (float)info.width / (float)info.height, 1.0, 100.0f);
		glUniformMatrix4fv(locs[1], 1, GL_FALSE, prj_matrix);
		mv_matrix = vmath::lookat(vmath::vec3(5.0f, 5.0f, 20.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(locs[0], 1, GL_FALSE, mv_matrix);
		glClearColor(0.0, 0.5, 0.5, 1.0);

		
		initNodes();

		theta[0] = 0.0f;
		theta[1] = 0.0f;
		theta[2] = 0.0f;
		theta[3] = 0.0f;
		theta[4] = 0.0f;
		theta[5] = 0.0f;
		theta[6] = 0.0f;
		theta[7] = 180.0f;
		theta[8] = 0.0f;
		theta[9] = 180.0f;
		theta[10] = 0.0f;
	}
	void render(float currentTime)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		traverse(&torso_node);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	}
protected:
	GLuint program;
	GLuint vao;
	GLuint buffers[2];
	GLuint locs[2];
	vmath::mat4 mv_matrix;
	vmath::mat4 prj_matrix;

	node torso_node, head_node, lua_node, rua_node,
		lll_node, rll_node, lla_node,
		rla_node, rul_node, lul_node;
};
int main(int argc, char** argv)
{
	robot_scene* a = new robot_scene;
	a->run(a);
	delete a;
	return 0;
}
