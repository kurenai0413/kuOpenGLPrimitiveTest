#version 410 core

in vec3 vertexColor;
in vec3 vertexPosition;
in vec3 vertexNormal;

out vec4 outColor;

uniform vec3 cameraPos;

void main()
{
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightPos   = cameraPos;
	vec3 objectColor = vec3(0.0f, 0.0f, 1.0f);
	
	// Ambient
	float ambientStrenth = 0.1f;
	vec3 ambient = ambientStrenth * lightColor;

	// Diffuse
	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPos - vertexPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular


	outColor = vec4((ambient + diffuse) * objectColor, 0.5f);
	
	//outColor = vec4(objectColor, 1.0f);
}
