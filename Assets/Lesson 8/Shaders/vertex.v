#version 330 core
#define MAX_WAVES 10
#define PI 3.14


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
uniform int numberOfWaves = 0;
uniform float waveSteepness[MAX_WAVES];
uniform float waveAmplitude[MAX_WAVES];
uniform float waveLength[MAX_WAVES];
uniform float waveSpeed[MAX_WAVES];
uniform vec2 waveDirection[MAX_WAVES];
uniform float time;



vec4 quaternionConjugate(vec4 quaternion);
vec3 quaternionRotate(vec3 position, vec4 quaternion);


void main()
{
	vec3 worldPosition	= quaternionRotate(position * objectScale, objectRotation) + objectPosition;
	vec3 offset = vec3(0.0);
	vec3 waveNormal = vec3(0.0, 0.0, 1.0);
	vec3 waveBitangent = vec3(1.0, 0.0, 0.0);
	vec3 waveTangent = vec3(0.0, 1.0, 0.0);
	for (int i = 0; i < numberOfWaves; i++)
	{
		vec2 direction = normalize(waveDirection[i]);
		
		float wavenumber = 2.0 * PI / waveLength[i];
		float arg = wavenumber * (dot(worldPosition.xy, direction) - waveSpeed[i] * time);
		float steepness = waveSteepness[i] / (wavenumber * waveAmplitude[i] * numberOfWaves);
		
		offset.xy += steepness * waveAmplitude[i] * direction * cos(arg);
		offset.z += waveAmplitude[i] * sin(arg);
		
		float kA = wavenumber * waveAmplitude[i];
		waveBitangent += vec3(
							-steepness * waveDirection[i].x * waveDirection[i].x * kA * sin(arg),
							-steepness * waveDirection[i].x * waveDirection[i].y * kA * sin(arg),
							waveDirection[i].x * kA * cos(arg));
		waveTangent += vec3(
							-steepness * waveDirection[i].x * waveDirection[i].y * kA * sin(arg),
							-steepness * waveDirection[i].y * waveDirection[i].y * kA * sin(arg),
							waveDirection[i].y * kA * cos(arg));
		waveNormal += vec3(
							-waveDirection[i].x * kA * cos(arg),
							-waveDirection[i].y * kA * cos(arg),
							-steepness * kA * sin(arg));
	}
	if (numberOfWaves > 0)
	{
		worldPosition += offset;
		waveBitangent = normalize(waveBitangent);
		waveTangent = normalize(waveTangent);
		waveNormal = normalize(waveNormal);
	}
	else
	{
		waveBitangent = tangent;
		waveTangent = tangent;
		waveNormal = normal;
	}
	
	vec3 viewPosition	= quaternionRotate(worldPosition - cameraPosition, quaternionConjugate(cameraRotation));
	vec3 worldNormal	= normalize(quaternionRotate(waveNormal / objectScale, objectRotation));
	
    gl_Position			= projection * vec4(viewPosition, 1.0);
	fragment.position	= worldPosition;
	fragment.texCoord	= texCoord;
	fragment.normal		= worldNormal;
	if (usingNormalMaps && normalMapsAllowed)
	{
		vec3 worldTangent	= normalize(quaternionRotate(waveTangent / objectScale, objectRotation));
		vec3 worldBitangent	= normalize(quaternionRotate(waveBitangent / objectScale, objectRotation));
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