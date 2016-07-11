#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
	vec4 objColor = texture(texture1, TexCoord);

	float ambiantStrength = 0.1f;
	vec3 ambiant = ambiantStrength * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDirect = normalize(lightPos - FragPos);
	float diff = max(dot(lightDirect, norm), 0.0f);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDirect, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 256);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = ambiant + diffuse + specular;
	color = objColor * vec4(result, 1.0f);
}