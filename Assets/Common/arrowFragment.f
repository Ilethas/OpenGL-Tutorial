#version 330 core


in Fragment
{
	vec3 position;
	vec3 normal;
	float depth;
} fragment;

out vec4 color;

uniform vec3 cameraPosition;
uniform vec3 objectColor;


void main()
{
	color = vec4((1.0 - fragment.depth) * objectColor, 1.0);
}
