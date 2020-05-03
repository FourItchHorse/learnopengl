#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 17) out;
uniform mat4 mvp;
uniform float sideLength = 1.0;
out vec4 fColor;
void generate_cube(vec4 orginalPos)
{
	vec3[8] my_positions = vec3[8]
						   (
						   vec3(-1.0), vec3(vec2(-1.0), 1.0), 
						   vec3(-1.0, 1.0, -1.0), vec3(-1.0, vec2(1.0)),
						   vec3(1.0, vec2(-1.0)), vec3(1.0, -1.0, 1.0),
						   vec3(vec2(1.0), -1.0), vec3(1.0)
						   );
	
	uint[17] my_indices = uint[17](0, 1, 2, 3, 6, 7, 4, 5,
								   0xFFFF, 
								   2, 6, 0, 4, 1, 3, 5, 7);

	for(uint i = 0; i < my_indices.length(); i++)
	{
		if (my_indices[i] < my_indices.length()) 
		{
		gl_Position = mvp * (orginalPos + vec4(my_positions[my_indices[i]], 0.0));
		fColor = vec4(my_positions[my_indices[i]], 1.0);
		EmitVertex();
		}
		else 
		{
		EndPrimitive();
		}
	}
	
}
void main (void)
{
	generate_cube(gl_in[0].gl_Position);
}