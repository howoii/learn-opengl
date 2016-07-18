#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 color;

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLight;
uniform DirLight dirLight;

uniform sampler2D Diffuse;
uniform sampler2D Specular;
uniform float repeat;
uniform float shininess;
uniform vec3 viewPos;

uniform sampler2D shadowMap;

vec3 PhongPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, float shadow){
	vec3 lightDir = normalize(vec3(light.position - FragPos));

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(Diffuse, TexCoords * repeat));
	vec3 diffuse = light.diffuse * vec3(texture(Diffuse, TexCoords * repeat)) * diff;
	vec3 specular = light.specular * vec3(texture(Specular, TexCoords * repeat)) * spec;

	diffuse *= (1.0-shadow);
	specular *= (1.0-shadow);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return (ambient + diffuse + specular) * attenuation;
}

vec3 BlinnPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, float shadow){
	vec3 lightDir = normalize(vec3(light.position - FragPos));

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 middleDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(middleDir, normal), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(Diffuse, TexCoords * repeat));
	vec3 diffuse = light.diffuse * vec3(texture(Diffuse, TexCoords * repeat)) * diff;
	vec3 specular = light.specular * vec3(texture(Specular, TexCoords * repeat)) * spec;

	diffuse *= (1.0-shadow);
	specular *= (1.0-shadow);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow){
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 middleDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(middleDir, normal), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(Diffuse, TexCoords * repeat));
	vec3 diffuse = light.diffuse * vec3(texture(Diffuse, TexCoords * repeat)) * diff;
	vec3 specular = light.specular * vec3(texture(Specular, TexCoords * repeat)) * spec;

	diffuse *= (1.0-shadow);
	specular *= (1.0-shadow);

	return (ambient + diffuse + specular);
}


float CalcShadow(DirLight light, vec4 fragPosLightSpace, vec3 normal){
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	vec3 lightDir = normalize(-light.direction);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = currentDepth - 0.005 > closestDepth ? 1.0 : 0.0;

	if (projCoords.z > 1.0)
		shadow = 0.0;
	return shadow;
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(vec3(viewPos - FragPos));

	float shadow = CalcShadow(dirLight, FragPosLightSpace, norm);

	vec3 Point = BlinnPointLight(pointLight, norm, FragPos, viewDir, shadow);
	vec3 Direct = CalcDirLight(dirLight, norm, viewDir, shadow);

	color = vec4(Direct, 1.0);
}