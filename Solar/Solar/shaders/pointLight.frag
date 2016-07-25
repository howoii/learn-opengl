#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform PointLight pointLight;
uniform float brightness;
uniform float shininess;
uniform vec3 viewPos;

uniform sampler2D diffuse;
uniform sampler2D specular;

out vec4 color;

vec3 BlinnPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);

void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(vec3(viewPos - FragPos));
	vec3 result = BlinnPointLight(pointLight, norm, FragPos, viewDir);
	color = vec4(result, 1.0);
}

vec3 BlinnPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir){
	vec3 lightDir = normalize(vec3(light.position - FragPos));

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 middleDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(middleDir, normal), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(diffuse, TexCoords));
	vec3 diffuse = light.diffuse * vec3(texture(diffuse, TexCoords)) * diff;
	vec3 specular = light.specular * vec3(texture(specular, TexCoords)) * spec;

	return (ambient + diffuse + specular) * brightness;;
}