#version 330 core


layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform vec4 cameraRotation;
uniform vec3 cameraPosition;

uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform vec3 objectScale;


vec4 quaternionConjugate(vec4 quaternion);
vec3 quaternionRotate(vec3 position, vec4 quaternion);


void main()
{

	vec3 worldPosition	= quaternionRotate(position * objectScale, objectRotation) + objectPosition;
	vec3 viewPosition	= quaternionRotate(worldPosition - cameraPosition, quaternionConjugate(cameraRotation));
    gl_Position			= projection * vec4(viewPosition, 1.0);
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
