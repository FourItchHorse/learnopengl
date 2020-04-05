#include "OGLScene.h"

class Sierpinski3D : public OGLScene
{
	const  int NDivisions = 5;
	const  static int NumVertices = 16384;
	vec3 points[NumVertices];

	int i = 0;
	void triangle(vec3& a, vec3& b, vec3& c)
	{
		points[i] = a;
		i++;
		points[i] = b;
		i++;
		points[i] = c;
		i++;
	}
	void tetra(vec3 &a, vec3 &b, vec3 &c, vec3 &d)
	{
		triangle(a, b, c);
		triangle(a, c, d);
		triangle(a, d, b);
		triangle(b, d, c);
	}
	void divide_tetra(vec3& a, vec3& b, vec3& c, vec3& d,  int k)
	{
		if (k > 0)
		{
			vec3 mid[6] = { (a + b) / 2.0, 
							(a + c) / 2.0, 
							(a + d) / 2.0, 
							(b + c) / 2.0,
							(c + d) / 2.0,
							(b + d) / 2.0 };

			divide_tetra(a, mid[0], mid[1], mid[2], k - 1);
			divide_tetra(mid[0], b, mid[3], mid[5], k - 1);
			divide_tetra(mid[1], mid[3], c, mid[4], k - 1);
			divide_tetra(mid[2], mid[5], mid[5], d, k - 1);
		}
		else tetra(a, b, c, d);
	}
	void startup()
	{
		program = glCreateProgram();
		const GLchar* vShaderSource = R"glsl(
		#version 450 core
		layout (location = 0) in vec3 vPosition;
		out VS_OUT
		{
			out vec4 color;
		}vs_out;
		layout (location = 2) uniform mat4 proj_matrix;
		layout (location = 3) uniform mat4 view_matrix;
		layout (location = 4) uniform mat4 model_matrix;
		void main()
		{
			gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vPosition, 1.0);
			vs_out.color = vec4(vPosition, 1.0) * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
		}
		)glsl";
		const GLchar* fShaderSource = R"glsl(
		#version 450 core
		out vec4 color;	
		in VS_OUT
		{
			in vec4 color;
		}fs_in;
		void main () 
		{
			color = fs_in.color;
		}
		)glsl";
		vec3 v[4] = { vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5), 
			            vec3(0.0, 0.5, -0.5), vec3(0.0, 0.0, 0.5) };
		divide_tetra(v[0], v[1], v[2], v[3], NDivisions);

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glCreateBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		compileShader(program, vShaderSource, GL_VERTEX_SHADER);
		compileShader(program, fShaderSource, GL_FRAGMENT_SHADER);
		glLinkProgram(program);
		glUseProgram(program);


		GLuint loc = glGetAttribLocation(program, "vPosition");
	
		proj = glGetAttribLocation(program, "proj_matrix");
		proj_matrix = perspective(50.0f, 800.0f/600.0f, 0.1f, 1000.0f);
		glUniformMatrix4fv(proj, 1, GL_FALSE, proj_matrix);

		view = glGetAttribLocation(program, "view_matrix");
	    view_matrix = mat4_lookat(vec3(0.0f, 2.0f, -5.0f), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(view, 1, GL_FALSE, view_matrix);

		mLoc = glGetAttribLocation(program, "model_matrix");
		model = MAT4_IDENTITY;
		glUniformMatrix4fv(mLoc, 1, GL_FALSE, model);


		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(loc);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.5, 0.5, 1.0);
		cameraY = 2.0f;
		glPointSize(15.0f);
	}

	void processInput(SDL_Event action)
	{
		if (action.type == SDL_KEYDOWN)
		{
			float increment = 0.1f;
			if (action.key.keysym.sym == SDLK_DOWN) cameraY += increment;
			if (action.key.keysym.sym == SDLK_UP) cameraY -= increment; 
			if (action.key.keysym.sym == SDLK_RIGHT) cameraX += increment;
			if (action.key.keysym.sym == SDLK_LEFT) cameraX -= increment;
			if (action.key.keysym.sym == SDLK_a) cameraZ -= increment;
			if (action.key.keysym.sym == SDLK_d) cameraZ += increment;
		}
	}
	void render(float currentTime)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(vao);
		glUseProgram(program);

		glUniformMatrix4fv(2, 1, GL_FALSE, proj_matrix);

		view_matrix = mat4_lookat(vec3(cameraX, cameraY, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(3, 1, GL_FALSE, view_matrix);

		/*model *= rotate(0.0, 1.0, 0.0, 0.001f) *
			rotate(0.0, 0.0, 1.0, 0.0001f);i */
		glUniformMatrix4fv(4, 1, GL_FALSE, model);

		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

private:
	GLuint vao;
	GLuint buffer;
	GLuint texture;
	GLuint program;
	GLuint mLoc;
	GLuint proj;
	GLuint view;
	mat4 proj_matrix;
	mat4 view_matrix;
	mat4 model;
	GLfloat cameraY;
	GLfloat cameraX;
	GLfloat cameraZ;
};

DECLARE_MAIN(Sierpinski3D);


int main(int argc, char** argv)
{
	Sierpinski3D* app = new Sierpinski3D();
	app->run(app);
	delete app;
	return 0;
}  
