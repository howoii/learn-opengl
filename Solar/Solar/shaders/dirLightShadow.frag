#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

struct DirLight
{
	vec3 direction;
	vec3 color;
	vec3 ambient;
};


struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform DirLight dirLight;
uniform Material material;
uniform vec3 viewPos;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D shadowMap;

out vec4 color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(vec3(viewPos - FragPos));
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	vec3 ambient = dirLight.ambient * vec3(texture(diffuse, TexCoords));

	float shadow = ShadowCalculation(FragPosLightSpace, Normal, normalize(-dirLight.direction));
	result = result * (1.0 - shadow) + ambient;
	color = vec4(result * dirLight.color, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 middleDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(middleDir, normal), 0.0), material.shininess);

	vec3 diffuse = material.diffuse * vec3(texture(diffuse, TexCoords)) * diff;
	vec3 specular = material.specular * vec3(texture(specular, TexCoords)) * spec;

	return (diffuse + specular);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}