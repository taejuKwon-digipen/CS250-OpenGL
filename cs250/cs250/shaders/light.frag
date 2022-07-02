#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;
uniform vec3 lightPos;
// uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 fogColor;
uniform vec3 camPos;

float fog() {
	float near = 2;
	float far = 4;

	float distance = length(lightPos - FragPos);
    // float distance = length(camPos - FragPos);

	float fogamount = smoothstep(near, far, distance);

	return fogamount;
}

void main() {
	float ambientStrength = 1.0;
	vec3 ambient = ambientStrength * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	/*float specularStrength = 0.5;
	vec3 viewDir = normalize(lightPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0, 0), 32);
	vec3 specular = specularStrength * spec * lightColor;*/

    vec3 result = mix(diffuse * objectColor, fogColor, fog());
    // vec3 result = mix(diffuse * objectColor, fogColor, 0.7);
    // vec3 result = diffuse * objectColor;

    FragColor = vec4(result, 1);
}