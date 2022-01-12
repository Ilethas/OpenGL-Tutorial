#version 330 core

#define PHONG 0
#define BLINN_PHONG 1

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
	vec4 rotation;
	vec3 direction;
	
	vec3 color;
	float brightness;
	float attenuationRadius;
	
	float innerConeAngle;
	float outerConeAngle;
	sampler2D shadowMap;
};


in Fragment
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	mat3 TBN;
} fragment;

out vec4 color;

uniform bool usingNormalMaps = false;

uniform vec3 cameraPosition;
uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform mat4 lightProjection;
uniform float shininess;
uniform int shadingType;


vec4 quaternionConjugate(vec4 quaternion);
vec3 quaternionRotate(vec3 position, vec4 quaternion);


vec3 calcLight(int lightIndex, int shadingType);
vec3 calcPointLight(int lightIndex, int shadingType);
vec3 calcDirectionalLight(int lightIndex, int shadingType);
vec3 calcSpotLight(int lightIndex, int shadingType);


void main()
{
	if (texture(material.diffuse[0], fragment.texCoord).a < 0.6)
	{
		discard;
	}
	
	
	color = vec4(vec3(0.0), 1.0);
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		if (lights[i].active)
		{
			float near = 1.0;
			float far = 100.0;
			vec3 pos = quaternionRotate(fragment.position - lights[i].position, quaternionConjugate(lights[i].rotation));
			float depth = (pos.x - near) / (far - near);
		
			vec4 lightSpacePosition = lightProjection * vec4(quaternionRotate(fragment.position - lights[i].position, quaternionConjugate(lights[i].rotation)), 1.0);
			vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
			projCoords = (projCoords + 1.0) / 2.0;
			
			float closestDepth = texture(lights[i].shadowMap, projCoords.xy).r;
			float currentDepth = projCoords.z;
			float shadow = currentDepth > closestDepth? 1.0 : 0.0;
			
			color += (1.0 - shadow) * vec4(calcLight(i, shadingType), 0.0);
		}
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
	vec3 normal;
	vec3 viewerDir = normalize(cameraPosition - fragment.position);
	vec3 lightDir = lights[lightIndex].position - fragment.position;
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);
	if (usingNormalMaps)
	{
		normal = normalize(fragment.TBN * (texture(material.normal[0], fragment.texCoord).rgb * 2.0 - 1.0));
	}
	else
	{
		normal = normalize(fragment.normal);
	}
	
	float specularDot;
	if (shadingType == BLINN_PHONG)
	{
		vec3 halfwayDir = normalize(viewerDir + lightDir);
		specularDot = dot(normal, halfwayDir);
	}
	else
	{
		vec3 reflected = reflect(-lightDir, normal);
		specularDot = dot(viewerDir, reflected);
	}
	
	float attenuation = clamp(1.0 - lightDistance*lightDistance/(lights[lightIndex].attenuationRadius*lights[lightIndex].attenuationRadius), 0.0, 1.0);
	attenuation *= attenuation;
	
	vec3 ambient = 0.1 * texture(material.diffuse[0], fragment.texCoord).rgb;
	vec3 diffuse = clamp(dot(normal, lightDir), 0.0, 1.0) * texture(material.diffuse[0], fragment.texCoord).rgb;
	vec3 specular = pow(clamp(specularDot, 0.0, 1.0), shininess) * texture(material.specular[0], fragment.texCoord).rgb;
	
	return (ambient + diffuse + specular) * lights[lightIndex].color * lights[lightIndex].brightness * attenuation;
}


vec3 calcDirectionalLight(int lightIndex, int shadingType)
{
	vec3 normal;
	vec3 viewerDir = normalize(cameraPosition - fragment.position);
	vec3 lightDir = normalize(-lights[lightIndex].direction);
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);
	if (usingNormalMaps)
	{
		normal = normalize(fragment.TBN * (texture(material.normal[0], fragment.texCoord).rgb * 2.0 - 1.0));
	}
	else
	{
		normal = normalize(fragment.normal);
	}
	
	float specularDot;
	if (shadingType == BLINN_PHONG)
	{
		vec3 halfwayDir = normalize(viewerDir + lightDir);
		specularDot = dot(normal, halfwayDir);
	}
	else
	{
		vec3 reflected = reflect(-lightDir, normal);
		specularDot = dot(viewerDir, reflected);
	}
	
	vec3 ambient = 0.1 * texture(material.diffuse[0], fragment.texCoord).rgb;
	vec3 diffuse = clamp(dot(normal, lightDir), 0.0, 1.0) * texture(material.diffuse[0], fragment.texCoord).rgb;
	vec3 specular = pow(clamp(specularDot, 0.0, 1.0), shininess) * texture(material.specular[0], fragment.texCoord).rgb;
	
	return (ambient + diffuse + specular) * lights[lightIndex].color * lights[lightIndex].brightness;
}


vec3 calcSpotLight(int lightIndex, int shadingType)
{
	vec3 normal;
	vec3 viewerDir = normalize(cameraPosition - fragment.position);
	vec3 lightDir = lights[lightIndex].position - fragment.position;
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);
	if (usingNormalMaps)
	{
		normal = normalize(fragment.TBN * (texture(material.normal[0], fragment.texCoord).rgb * 2.0 - 1.0));
	}
	else
	{
		normal = normalize(fragment.normal);
	}
	
	float specularDot;
	if (shadingType == BLINN_PHONG)
	{
		vec3 halfwayDir = normalize(viewerDir + lightDir);
		specularDot = dot(normal, halfwayDir);
	}
	else
	{
		vec3 reflected = reflect(-lightDir, normal);
		specularDot = dot(viewerDir, reflected);
	}
	
	float attenuation = clamp(1.0 - lightDistance*lightDistance/(lights[lightIndex].attenuationRadius*lights[lightIndex].attenuationRadius), 0.0, 1.0);
	attenuation *= attenuation;
	
	vec3 ambient = 0.1 * texture(material.diffuse[0], fragment.texCoord).rgb;
	vec3 diffuse = clamp(dot(normal, lightDir), 0.0, 1.0) * texture(material.diffuse[0], fragment.texCoord).rgb;
	vec3 specular = pow(clamp(specularDot, 0.0, 1.0), shininess) * texture(material.specular[0], fragment.texCoord).rgb;
	
	float theta = dot(-lightDir, normalize(lights[lightIndex].direction));
	float spotLightFactor = smoothstep(cos(lights[lightIndex].outerConeAngle / 2.0), cos(lights[lightIndex].innerConeAngle / 2.0), theta);
	return (ambient + diffuse + specular) * lights[lightIndex].color * lights[lightIndex].brightness * attenuation * spotLightFactor;
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