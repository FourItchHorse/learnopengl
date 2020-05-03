/*
#include "SDLBase.h"
//#include "ShaderReader.h"


enum BUFFER_TYPE_t
{
	POSITION_A,
	POSITION_B,
	VELOCITY_A,
	VELOCITY_B,
	CONNECTION
};

enum
{
	POINTS_X			= 50,
	POINTS_Y			= 50,
	POINTS_TOTAL		= (POINTS_X * POINTS_Y),
	CONNECTIONS_TOTAL	= (POINTS_X - 1) * POINTS_Y + (POINTS_Y - 1) * POINTS_X

};


class SpringSystem : public SDLOGL
{
public:
	void init()
	{

		ShaderInfo updateShaders[]
		{
			{GL_VERTEX_SHADER, "0105springmassUpdateVert.glsl"},
			{GL_FRAGMENT_SHADER, "0105springmassFrag.glsl"},
			{GL_NONE, NULL}
		};

		updateProgram = LoadShaders(updateShaders);

		ShaderInfo renderShaders[]
		{
			{GL_VERTEX_SHADER, "0105springmassVert.glsl"},
			{GL_FRAGMENT_SHADER, "0105springmassFrag.glsl"},
			{GL_NONE, NULL}
		};

		renderProgram = LoadShaders(renderShaders);

		glUseProgram(updateProgram);

		int i, j;

		vmath::vec4 * initial_positions = new vmath::vec4[POINTS_TOTAL];
		vmath::vec3* initial_velocities = new vmath::vec3[POINTS_TOTAL];
		vmath::ivec4* connection_vectors = new vmath::ivec4[POINTS_TOTAL];


		int n = 0;

		for (j = 0; j < POINTS_Y; j++)
		{
			float fj = (float)j / (float)POINTS_Y;
			for (i = 0; i < POINTS_X; i++)
			{
				float fi = (float)i / (float)POINTS_X;

				initial_positions[n] = vmath::vec4((fi - 0.5f) * (float)POINTS_X,
													(fj - 0.5f) * (float)POINTS_Y,
													0.6f * sinf(fi) * cos(fj), 1.0f);
				initial_velocities[n] = vmath::vec3(0.0f);

				connection_vectors[n] = vmath::ivec4(-1);

				if (j != (POINTS_Y - 1))
				{
					if (i != 0)
						connection_vectors[n][0] = n - 1;
					
					if (j != 0)
						connection_vectors[n][1] = n - POINTS_X;

					if (i != (POINTS_X - 1))
						connection_vectors[n][2] = n + 1;

					if (j != (POINTS_Y - 1))
						connection_vectors[n][3] = n + POINTS_X;
				}
				n++;
			}
		}

		glGenVertexArrays(2, vaos);
		glGenBuffers(5, vbos);
		printf("Generating arrays and buffers: %i\n", glGetError());

		for (int i = 0; i < 2; i++)
		{
			glBindVertexArray(vaos[i]);

			glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_A + i]);
			glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(vmath::vec4), initial_positions, GL_DYNAMIC_COPY);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vbos[VELOCITY_A + i]);
			glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(vmath::vec3), initial_velocities, GL_DYNAMIC_COPY);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, vbos[CONNECTION]);
			glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(vmath::ivec4), connection_vectors, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 4, GL_INT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(2);
		}

		glGenVertexArrays(2, vaos);
		glGenBuffers(5, vbos);
		printf("Writing to buffers: %i\n", glGetError());
		
		delete[] connection_vectors;
		delete[] initial_positions;
		delete[] initial_velocities;

		glGenTextures(2, pos_tbo);
		glBindTexture(GL_TEXTURE_BUFFER, pos_tbo[0]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbos[POSITION_A]);
		glBindTexture(GL_TEXTURE_BUFFER, pos_tbo[1]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbos[POSITION_B]);

	}
	void display()
	{
		int i;
		glUseProgram(updateProgram);

		glEnable(GL_RASTERIZER_DISCARD);
		for (i = iterations_per_frame; i != 0; --i)
		{
			glBindVertexArray(vaos[iterationIndex & 1]);
			glBindTexture(GL_TEXTURE_BUFFER, pos_tbo[iterationIndex & 1]);
			iterationIndex++;
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,
							 vbos[POSITION_A + (iterationIndex & 1)]);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1,
							 vbos[VELOCITY_A + (iterationIndex & 1)]);
			glBeginTransformFeedback(GL_POINTS);
			glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
			glEndTransformFeedback();
		}
		glDisable(GL_RASTERIZER_DISCARD);

		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glViewport(0, 0, info.width, info.height);
		glClearBufferfv(GL_COLOR, 0, black);
		glUseProgram(renderProgram);
		glPointSize(4.0f);
		glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void shutdown()
	{
	}
protected:
	GLuint vaos[2];
	GLuint vbos[5];
	GLuint pos_tbo[2];
	GLuint renderProgram;
	GLuint updateProgram;
	GLuint iterationIndex;
	int iterations_per_frame = 2;
};


/*
int main(int argc, char** argv)
{
	SpringSystem* a = new SpringSystem;
	a->run();
	delete a;
	return 0;
}  */ 