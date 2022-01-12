#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out Fragment
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	mat3 TBN;
} fragment;

uniform mat4 projection;
uniform vec4 cameraRotation;
uniform vec3 cameraPosition;

uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform vec3 objectScale;

uniform bool normalMapsAllowed = true;
uniform bool usingNormalMaps = false;


vec4 quaternionConjugate(vec4 quaternion);
vec3 quaternionRotate(vec3 position, vec4 quaternion);


void main()
{
	vec3 worldPosition	= quaternionRotate(position * objectScale, objectRotation) + objectPosition;
	vec3 viewPosition	= quaternionRotate(worldPosition - cameraPosition, quaternionConjugate(cameraRotation));
	vec3 worldNormal	= normalize(quaternionRotate(normal / objectScale, objectRotation));
	
    gl_Position			= projection * vec4(viewPosition, 1.0);
	fragment.position	= worldPosition;
	fragment.texCoord	= texCoord;
	fragment.normal		= worldNormal;
	if (usingNormalMaps && normalMapsAllowed)
	{
		vec3 worldTangent	= normalize(quaternionRotate(tangent / objectScale, objectRotation));
		vec3 worldBitangent	= normalize(quaternionRotate(bitangent / objectScale, objectRotation));
		if (dot(cross(worldNormal, worldTangent), worldBitangent) < 0.0)
		{
			worldTangent *= -1.0;
		}
		
		worldTangent	= normalize(worldTangent - dot(worldTangent, worldNormal) * worldNormal);
		worldBitangent	= cross(worldNormal, worldTangent);
		fragment.TBN	= mat3(worldTangent, worldBitangent, worldNormal);
	}
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