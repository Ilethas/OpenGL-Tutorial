#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out Fragment
{
	vec2 texCoord;
} fragment;

void main()
{
    gl_Position			= vec4(position, 1.0);
	fragment.texCoord	= texCoord;
}