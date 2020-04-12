#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
GLfloat vertices[][3] = {{-1.0, -1.0, 1.0},{-1.0, 1.0, 1.0},{1.0, 1.0, 1.0},{1.0, -1.0, 1.0},
		     {-1.0, -1.0, -1.0},{-1.0, 1.0, -1.0},{1.0, 1.0, -1.0},{1.0, -1.0, -1.0}};
GLfloat colors[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
		       {1.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}};
GLubyte cubeIndices[] = {0, 3, 2, 1, 2, 3, 7, 6, 3, 0, 4, 7, 1, 2, 6, 5, 4, 5, 6, 7, 5, 4, 0, 1};
GLfloat cameraPos[3] = {-8.0, 5.0, 5.0};
float theta[11];
int angle;
typedef struct treenode
{
    GLfloat m[16];
    void(*f)();
    struct treenode *sibling;
    struct treenode *child;
}treenode;
void traverse(treenode *root)
{
    if(root == NULL)return;
    glPushMatrix();
    glMultMatrixf(root->m);
    root ->f();
    if(root->child != NULL)traverse(root->child);
    glPopMatrix();
    if(root->sibling!= NULL)traverse(root->sibling);
}
typedef treenode* tree_ptr;

treenode torso_node, head_node, lua_node, lla_node, rua_node, 
            rla_node, lul_node, lll_node, rul_node, rll_node;

void torso();void head(); 
void left_upper_arm();void left_lower_arm();
void right_upper_arm();void right_lower_arm();
void left_upper_leg();void left_lower_leg();
void right_upper_leg();void right_lower_leg();
void torso()
{
    glScalef(1.0, 5.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void head()
{
    glTranslatef(1.0, 1.5, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void left_upper_arm()
{
    glScalef(0.75, 2.0, 0.75);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void left_lower_arm()
    glScalef(0.75, 2.0, 0.75);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void right_upper_arm()
{
    glScalef(0.75, 2.0, 0.75);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void right_lower_arm()
{
    glScalef(0.75, 2.0, 0.75);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void left_upper_leg()
{
    glScalef(0.5, 2.0, 0.5);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void left_lower_leg()
{
    glScalef(0.5, 2.0, 0.5);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void right_upper_leg()
    glScalef(0.5, 2.0, 0.5);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}
void right_lower_leg()
{
    glScalef(0.5, 2.0, 0.5);
    glTranslatef(1.0, -2.0, 1.0);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
}

void myInit()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLoadIdentity();
    glRotatef(theta[0], 0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);
    torso_node.f = torso;
    torso_node.sibling = NULL;
    torso_node.child = &head_node;

    glLoadIdentity();
    glTranslatef(0.0, 5.5, 0.0);
    glRotatef(theta[1], 1.0, 0.0, 0.0);
    glRotatef(theta[2], 0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, head_node.m);
    head_node.f = head;
    head_node.sibling = &lua_node;
    head_node.child = NULL;

    glLoadIdentity();
    glTranslatef(-1.75, 4.5, 0.0);
    glRotatef(theta[3], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);
    lua_node.f = left_upper_arm;
    lua_node.sibling = &rua_node;
    lua_node.child = &lla_node;

    glLoadIdentity();
    glTranslatef(0.0, 2.0, 0.0);
    glRotatef(theta[4], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);
    lla_node.f = left_lower_arm;
    lla_node.sibling = NULL;
    lla_node.child = NULL;

    glLoadIdentity();
    glTranslatef(1.75, 4.5, 0.0);
    glRotatef(theta[5], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);
    rua_node.f = right_upper_arm;
    rua_node.sibling = &lul_node;
    rua_node.child = &rla_node;
      
    glLoadIdentity();
    glTranslatef(0.0, 2.0, 0.0);
    glRotatef(theta[6], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);
    rla_node.f = right_lower_arm;
    rla_node.sibling = NULL;
    rla_node.child = NULL;

    glLoadIdentity();
    glTranslatef(-1.5, 0.2, 0.0);
    glRotatef(theta[7], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);
    lul_node.f = left_upper_leg;
    lul_node.sibling = &rul_node;
    lul_node.sibling = &lll_node;

    glLoadIdentity();
    glTranslatef(0.0, 2.0, 0.0);
    glRotatef(theta[8], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);
    lll_node.f = left_lower_leg;
    lll_node.sibling = NULL;
    lll_node.child = NULL;

    glLoadIdentity();
    glTranslatef(1.5, 0.2, 0.0);
    glRotatef(theta[9], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);
    rul_node.f = right_upper_leg;
    rul_node.sibling = NULL;
    rul_node.child = &rll_node;

    glLoadIdentity();
    glTranslatef(0.0, 2.0, 0.0);
    glRotatef(theta[10], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);
    rll_node.f = right_lower_leg;
    rll_node.sibling = NULL;
    rll_node.child = NULL;

    glLoadIdentity();
}
void mouse(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
        {
        theta[angle] += 0.05;
        if( theta[angle] > 360.0 ) theta[angle] -= 360.0;
        }
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
        {
        theta[angle] -= 0.05;
        if( theta[angle] < 360.0 ) theta[angle] += 360.0;
        }
}   
void menu(int id)
{
   if(id <11 ) angle=id;
   if(id ==11 ) exit(0);
}
void reshape(int w, int h)
{
     glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-10.0, 10.0, -10.0 * (GLfloat) h / (GLfloat) w,
            10.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-10.0 * (GLfloat) w / (GLfloat) h,
            10.0 * (GLfloat) w / (GLfloat) h, 0.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    traverse(&torso_node);
    glutSwapBuffers();
}
void myKeys(unsigned char key, int x, int y)
{
	float increment = 0.1;
	if(key == 'a')cameraPos[0] -= increment;
	if(key == 'd')cameraPos[0] += increment;
	if(key == 'w')cameraPos[1] -= increment;
	if(key == 's')cameraPos[1] += increment;
    if(key == 'q')cameraPos[2] -= increment;
    if(key == 'e')cameraPos[2] += increment;
	printf("%f, %f, %f\r", cameraPos[0], cameraPos[1], cameraPos[2]);
	glutPostRedisplay();
}
int main (int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("robot");
    myInit();
    glutKeyboardFunc(myKeys);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutCreateMenu(menu);
    glutAddMenuEntry("torso", 0);
    glutAddMenuEntry("head1", 1);
    glutAddMenuEntry("head2", 2);
    glutAddMenuEntry("right_upper_arm", 3);
    glutAddMenuEntry("right_lower_arm", 4);
    glutAddMenuEntry("left_upper_arm", 5);
    glutAddMenuEntry("left_lower_arm", 6);
    glutAddMenuEntry("right_upper_leg", 7);
    glutAddMenuEntry("right_lower_leg", 8);
    glutAddMenuEntry("left_upper_leg", 9);
    glutAddMenuEntry("left_lower_leg", 10);
    glutAddMenuEntry("quit", 11);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutMainLoop();
}
