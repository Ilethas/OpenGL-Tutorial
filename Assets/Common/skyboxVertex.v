#version 330 core


layout (location = 0) in vec3 position;

out Fragment
{
	vec3 texCoord;
} fragment;

uniform mat4 projection;
uniform vec4 cameraRotation;


vec4 quaternionConjugate(vec4 quaternion);
vec3 quaternionRotate(vec3 position, vec4 quaternion);


void main()
{
	mat3 cubemapSpace = mat3(
		0.0, 0.0, -1.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);
	vec3 viewPosition	= quaternionRotate(position, quaternionConjugate(cameraRotation));
    gl_Position			= projection * vec4(viewPosition, 1.0);
	fragment.texCoord	= cubemapSpace * position;
}


vec4 quaternionConjugate(vec4 quaternion)
{
	return vec4(-quaternion.xyz, quaternion.w);
}


vec3 quaternionRotate(vec3 position, vec4 quaternion)
{
	vec3 n = cross(quaternion.xyz, position);
	return 2*quaternion.w*n + 2*cross(quaternion.xyz, n) + position;
}
