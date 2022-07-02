#version 330

out vec4 FragColor;

in vec3 gNormal;

uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;

void main()
{
	vec3 N = normalize(gNormal);
	vec3 L = normalize(LightPosition);
	float nl = max(dot(N,L), 0);

	vec3 color = nl*DiffuseMaterial+AmbientMaterial;

	FragColor = vec4(color, 1);
}