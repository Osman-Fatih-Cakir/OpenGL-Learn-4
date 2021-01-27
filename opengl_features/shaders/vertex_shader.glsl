#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vColor;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;



out vec4 fColor;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	fColor = vColor;
}