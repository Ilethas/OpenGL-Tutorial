#version 330 core


in Fragment
{
	vec3 color;
} fragment;

out vec4 color;


void main()
{
	color = vec4(1.0, 1.0, 0.0, 1.0);
}
