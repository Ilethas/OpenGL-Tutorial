#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out Fragment
{
	vec3 position;
	vec3 normal;
	float depth;
} fragment;

uniform float nearPlane;
uniform float farPlane;
uniform vec3 offset;
uniform mat4 projection;
uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform vec3 objectScale;


vec4 quaternionConjugate(vec4 quaternion);
vec3 quaternionRotate(vec3 position, vec4 quaternion);

void main()
{
	vec3 worldPosition	= quaternionRotate((position + offset) * objectScale, objectRotation) + objectPosition;
	vec3 worldNormal	= normalize(quaternionRotate(normal / objectScale, objectRotation));
	
    gl_Position			= projection * vec4(worldPosition, 1.0);
	fragment.position	= worldPosition;
	fragment.normal		= worldNormal;
	fragment.depth		= clamp((worldPosition.x - nearPlane) / (farPlane - nearPlane), 0.0, 1.0);
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
