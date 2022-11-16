#version 330 core

// layout
layout (location = 0) in vec3 layoutPosition;

// uniform
uniform mat4 model;
uniform mat4 cameraMatrix;

void main()
{
	gl_Position = cameraMatrix * model * vec4(layoutPosition, 1.0f);
}