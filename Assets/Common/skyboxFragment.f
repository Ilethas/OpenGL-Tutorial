#version 330 core


in Fragment
{
	vec3 texCoord;
} fragment;

out vec4 color;

uniform samplerCube skybox;


void main()
{    
    color = texture(skybox, fragment.texCoord);
}
