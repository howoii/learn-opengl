#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 color;

struct Light
{
	vec3 position;
	vec3 spotDir;
	float outter;
	float inner;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;
uniform Light light;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main(){
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear*distance + light.quadratic*(distance*distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	
	vec3 norm = normalize(Normal);
	vec3 lightDirect = normalize(light.position - FragPos);
	//vec3 lightDirect = normalize(-light.direction);
	float diff = max(dot(lightDirect, norm), 0.0f);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoord));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDirect, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoord));

	
	float spot = dot(normalize(light.spotDir), -lightDirect);
	attenuation *= clamp((spot - light.outter)/(light.inner - light.outter), 0.0f, 1.0f);

	vec3 result = ambient + (diffuse + specular) * attenuation;
	color = vec4(result, 1.0f) * vec4(lightColor, 1.0f);
}