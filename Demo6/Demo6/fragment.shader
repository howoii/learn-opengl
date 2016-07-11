#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 color;

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
	//vec4 objColor = texture(texture1, TexCoord);
	//vec4 objColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	vec3 ambiant = light.ambient * vec3(texture(material.diffuse, TexCoord));
	
	vec3 norm = normalize(Normal);
	vec3 lightDirect = normalize(light.position - FragPos);
	float diff = max(dot(lightDirect, norm), 0.0f);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoord));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDirect, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoord));

	vec3 result = ambiant + diffuse + specular;
	color = vec4(result, 1.0f);
}