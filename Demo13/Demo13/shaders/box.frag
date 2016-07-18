#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

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

uniform PointLight pointLight;

uniform float repeat;
uniform sampler2D Diffuse;
uniform sampler2D Specular;
uniform float shininess;
uniform vec3 viewPos;

vec3 PhongPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir){
	vec3 lightDir = normalize(vec3(light.position - FragPos));

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(Diffuse, TexCoords * repeat));
	vec3 diffuse = light.diffuse * vec3(texture(Diffuse, TexCoords * repeat)) * diff;
	vec3 specular = light.specular * vec3(texture(Specular, TexCoords * repeat)) * spec;

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return (ambient + diffuse + specular) * attenuation;
}

vec3 BlinnPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir){
	vec3 lightDir = normalize(vec3(light.position - FragPos));

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 middleDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(middleDir, normal), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(Diffuse, TexCoords * repeat));
	vec3 diffuse = light.diffuse * vec3(texture(Diffuse, TexCoords * repeat)) * diff;
	vec3 specular = light.specular * vec3(texture(Specular, TexCoords * repeat)) * spec;

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return (ambient + diffuse + specular) * attenuation;
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(vec3(viewPos - FragPos));

	vec3 result;
	if (gl_FragCoord.x < 400)
		result = BlinnPointLight(pointLight, norm, FragPos, viewDir);
	else
		result = PhongPointLight(pointLight, norm, FragPos, viewDir);
		
	color = vec4(result, 1.0);
}