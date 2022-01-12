#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out Fragment
{
	vec4 normal;
} fragment;

uniform mat4 projection;
uniform vec4 cameraRotation;
uniform vec3 cameraPosition;

uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform vec3 objectScale;

uniform bool usingNormalMaps = false;


vec4 quaternionConjugate(vec4 quaternion);
vec3 quaternionRotate(vec3 position, vec4 quaternion);


void main()
{
	vec3 worldPosition	= quaternionRotate(position * objectScale, objectRotation) + objectPosition;
	vec3 viewPosition	= quaternionRotate(worldPosition - cameraPosition, quaternionConjugate(cameraRotation));
	vec3 worldNormal	= normalize(quaternionRotate(normal / objectScale, objectRotation));
	vec3 viewNormal		= quaternionRotate(worldNormal, quaternionConjugate(cameraRotation));
	
    gl_Position			= projection * vec4(viewPosition, 1.0);
	fragment.normal		= normalize(projection * vec4(viewNormal, 0.0));
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
