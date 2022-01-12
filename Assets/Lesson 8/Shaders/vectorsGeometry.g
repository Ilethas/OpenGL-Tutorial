#version 330 core


layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in Fragment
{
	vec4 normal;
} fragmentIn[];


void main()
{
	float lineLength = 5.25;
	for (int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
		
		gl_Position = gl_in[i].gl_Position + fragmentIn[i].normal * lineLength;
		EmitVertex();
		EndPrimitive();
	}
}
