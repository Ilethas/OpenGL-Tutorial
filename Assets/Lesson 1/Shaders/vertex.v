#version 330 core

#define PHONG 0
#define BLINN_PHONG 1

#define FLAT 0
#define SMOOTH 1
#define GOURAUD 2

#define MAX_DIFFUSE_MAPS 1
#define MAX_SPECULAR_MAPS 1
#define MAX_NORMAL_MAPS 1
#define MAX_LIGHTS 25

#define PointLight 0
#define DirectionalLight 1
#define SpotLight 2


struct Material
{
	sampler2D diffuse[MAX_DIFFUSE_MAPS];
	sampler2D specular[MAX_SPECULAR_MAPS];
	sampler2D normal[MAX_NORMAL_MAPS];
};

struct Light
{
	bool active;
	int lightType;
	vec3 position;
	vec3 direction;
	
	vec3 color;
	float brightness;
	float attenuationRadius;
	
	float innerConeAngle;
	float outerConeAngle;
};


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out Fragment
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec3 color;
} fragment;

uniform mat4 projection;
uniform vec4 cameraRotation;
uniform vec3 cameraPosition;

uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform vec3 objectScale;

uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform float shininess;
uniform int interpolationType;
uniform int shadingType;


vec3 calcLight(int lightIndex, int shadingType);
vec3 calcPointLight(int lightIndex, int shadingType);
vec3 calcDirectionalLight(int lightIndex, int shadingType);
vec3 calcSpotLight(int lightIndex, int shadingType);

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
	
	if (interpolationType != SMOOTH)
	{
		vec3 color = vec3(0.0);
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (lights[i].active)
			{
				color += calcLight(i, shadingType);
			}
		}
		fragment.color = color;
	}
}


vec3 calcLight(int lightIndex, int shadingType)
{
	switch(lights[lightIndex].lightType)
	{
		case PointLight:		return calcPointLight(lightIndex, shadingType);
		case DirectionalLight:	return calcDirectionalLight(lightIndex, shadingType);
		case SpotLight:			return calcSpotLight(lightIndex, shadingType);			
	}
	return vec3(0);
}


vec3 calcPointLight(int lightIndex, int shadingType)
{
	vec3 viewerDir = normalize(cameraPosition - fragment.position);
	vec3 lightDir = lights[lightIndex].position - fragment.position;
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);
	
	float specularDot;
	if (shadingType == BLINN_PHONG)
	{
		vec3 halfwayDir = normalize(viewerDir + lightDir);
		specularDot = dot(fragment.normal, halfwayDir);
	}
	else
	{
		vec3 reflected = reflect(-lightDir, fragment.normal);
		specularDot = dot(viewerDir, reflected);
	}
	
	float attenuation = clamp(1.0 - lightDistance*lightDistance/(lights[lightIndex].attenuationRadius*lights[lightIndex].attenuationRadius), 0.0, 1.0);
	attenuation *= attenuation;
	
	vec3 ambient = vec3(0.1);
	vec3 diffuse = vec3(clamp(dot(fragment.normal, lightDir), 0.0, 1.0));
	vec3 specular = pow(clamp(specularDot, 0.0, 1.0), shininess) * texture(material.specular[0], fragment.texCoord).rgb;
	
	return (ambient + diffuse + specular) * lights[lightIndex].brightness * attenuation * lights[lightIndex].color;
}


vec3 calcDirectionalLight(int lightIndex, int shadingType)
{
	vec3 viewerDir = normalize(cameraPosition - fragment.position);
	vec3 lightDir = normalize(-lights[lightIndex].direction);
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);
	
	float specularDot;
	if (shadingType == BLINN_PHONG)
	{
		vec3 halfwayDir = normalize(viewerDir + lightDir);
		specularDot = dot(fragment.normal, halfwayDir);
	}
	else
	{
		vec3 reflected = reflect(-lightDir, fragment.normal);
		specularDot = dot(viewerDir, reflected);
	}
	
	vec3 ambient = vec3(0.1);
	vec3 diffuse = vec3(clamp(dot(fragment.normal, lightDir), 0.0, 1.0));
	vec3 specular = pow(clamp(specularDot, 0.0, 1.0), shininess) * texture(material.specular[0], fragment.texCoord).rgb;
	
	return (ambient + diffuse + specular) * lights[lightIndex].color * lights[lightIndex].brightness * lights[lightIndex].color;
}


vec3 calcSpotLight(int lightIndex, int shadingType)
{
	vec3 viewerDir = normalize(cameraPosition - fragment.position);
	vec3 lightDir = lights[lightIndex].position - fragment.position;
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);
	
	float specularDot;
	if (shadingType == BLINN_PHONG)
	{
		vec3 halfwayDir = normalize(viewerDir + lightDir);
		specularDot = dot(fragment.normal, halfwayDir);
	}
	else
	{
		vec3 reflected = reflect(-lightDir, fragment.normal);
		specularDot = dot(viewerDir, reflected);
	}
	
	float attenuation = clamp(1.0 - lightDistance*lightDistance/(lights[lightIndex].attenuationRadius*lights[lightIndex].attenuationRadius), 0.0, 1.0);
	attenuation *= attenuation;
	
	vec3 ambient = vec3(0.1);
	vec3 diffuse = vec3(clamp(dot(fragment.normal, lightDir), 0.0, 1.0));
	vec3 specular = pow(clamp(specularDot, 0.0, 1.0), shininess) * texture(material.specular[0], fragment.texCoord).rgb;
	
	float theta = dot(-lightDir, normalize(lights[lightIndex].direction));
	float spotLightFactor = smoothstep(cos(lights[lightIndex].outerConeAngle / 2.0), cos(lights[lightIndex].innerConeAngle / 2.0), theta);
	return (ambient + diffuse + specular) * lights[lightIndex].brightness * attenuation * spotLightFactor * lights[lightIndex].color;
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