#version 410 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
in vec3 color;

out vec3 vertexColor;
out vec3 vertexNormal;
out vec3 vertexPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	vertexNormal = normal;
	vertexPosition = position;
	vertexColor = color;
}