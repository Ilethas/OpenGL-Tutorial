#version 330 core
in Fragment
{
	vec2 texCoord;
} fragment;
out vec4 color;
uniform sampler2D renderedTexture;
uniform float threshold;


void main()
{
	vec3 originalPixel = texture(renderedTexture, fragment.texCoord).rgb;

    vec3 brightPixel = vec3(0.0, 0.0, 0.0);
    float brightness = dot(originalPixel, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > (1.0 - threshold))
        brightPixel = vec3(originalPixel);
	color = vec4(brightPixel, 1.0);
}