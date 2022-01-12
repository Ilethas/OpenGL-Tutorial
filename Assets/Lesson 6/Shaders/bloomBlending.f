#version 330 core

in Fragment
{
	vec2 texCoord;
} fragment;
out vec4 color;
uniform sampler2D bloomBlur;
uniform sampler2D image;

void main( void )
{ 
	vec3 A = texture(bloomBlur, fragment.texCoord).rgb;		//rozmycie
	vec3 B = texture(image, fragment.texCoord).rgb;			//obraz
   
	vec3 result = 1.0 - ((1.0-A)*(1.0-B));//screen
	//vec3 result = B/(1.0-A);	//Color Dodge
	//vec3 result = B;
	color = vec4(result, 1.0);
}