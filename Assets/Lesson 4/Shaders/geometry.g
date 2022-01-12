#version 330 core

#define PHONG 0
#define BLINN_PHONG 1

#define FLAT 0
#define SMOOTH 1
#define GOURAUD 2


layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Fragment
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec3 color;
} fragmentIn[];

out Fragment
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec3 color;
} fragmentOut;

uniform int interpolationType;
uniform int shadingType;


void main()
{
	for (int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		if (interpolationType == FLAT)
		{
			vec3 a = fragmentIn[2].position - fragmentIn[1].position;
			vec3 b = fragmentIn[0].position - fragmentIn[1].position;
			vec3 normal = normalize(cross(a, b));
			vec3 position = (fragmentIn[0].position + fragmentIn[1].position + fragmentIn[2].position) / 3.0;
	
			fragmentOut.position = position;
			fragmentOut.normal = normal;
		}
		else
		{
			fragmentOut.position = fragmentIn[i].position;
			fragmentOut.normal = fragmentIn[i].normal;
		}
		fragmentOut.texCoord = fragmentIn[i].texCoord;
		fragmentOut.color = fragmentIn[i].color;
		EmitVertex();
	}
	
	EndPrimitive();
}
