#version 330 core

in Fragment
{
	vec2 texCoord;
} fragment;
out vec4 color;
uniform sampler2D renderedTexture;

//uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

//uniform float weight[11] = float[] (0.100346,	0.097274,	0.088613,	0.075856,	0.061021,	0.046128,	0.032768,	0.021874,	0.013722,	0.008089, 0.004481);

uniform float weight[26] = float[] (0.040312,	0.040111,	0.039514,	0.038539,	0.037215,	0.035579,	0.033676,	0.031559,	0.02928,	0.026896,	0.024461,	0.022024,	0.019634,	0.017328,	0.015142,	0.0131,	0.01122,	0.009515,	0.007988,	0.00664,	0.005465,	0.004453,	0.003592,	0.002869,	0.002268,	0.001776);

void main()
{     
    vec3 originalPixel = texture(renderedTexture, fragment.texCoord).rgb;
	vec2 tex_offset = 1.0 / textureSize(renderedTexture, 0);
	vec3 result = texture(renderedTexture, fragment.texCoord).rgb * weight[0];
	for(int i = 1; i < 26; ++i)
	{
		result += texture(renderedTexture, fragment.texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		result += texture(renderedTexture, fragment.texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
	}
    color = vec4(result, 1.0);
}