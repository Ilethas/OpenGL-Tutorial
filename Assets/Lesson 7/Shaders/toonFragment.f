#version 330 core


in Fragment
{
	vec2 texCoord;
} fragment;
out vec4 color;
uniform sampler2D renderedTexture;


const float offset = 1.0 / 200.0;
vec2 offsets[9] = vec2[](
	vec2(-offset,  offset), // top-left
	vec2( 0.0f,    offset), // top-center
	vec2( offset,  offset), // top-right
	vec2(-offset,  0.0f),   // center-left
	vec2( 0.0f,    0.0f),   // center-center
	vec2( offset,  0.0f),   // center-right
	vec2(-offset, -offset), // bottom-left
	vec2( 0.0f,   -offset), // bottom-center
	vec2( offset, -offset)  // bottom-right
);
float horizontalKernel[9] = float[](
	-1.0, 0.0, 1.0,
	-2.0, 0.0, 2.0,
	-1.0, 0.0, 1.0
);
float verticalKernel[9] = float[](
	-1.0, -2.0, -1.0,
	 0.0,  0.0,  0.0,
	 1.0,  2.0,  1.0
);
float threshold = 1.0;


float intensity(vec3 color);


void main()
{
	vec3 fragColor = texture(renderedTexture, fragment.texCoord).rgb;

	float horizontal = 0.0;
	float vertical = 0.0;
	for (int i = 0; i < 9; i++)
	{
		float sampledTexture = intensity(texture(renderedTexture, fragment.texCoord + offsets[i]).rgb);
		horizontal += sampledTexture * horizontalKernel[i];
		vertical += sampledTexture * verticalKernel[i];
	}
	vec2 gradient = vec2(horizontal, vertical);
	
	color = vec4(fragColor, 1.0);
	if (length(gradient) > threshold)
	{
		color = vec4(vec3(0.0), 1.0);
	}
}


float intensity(vec3 color)
{
	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}
