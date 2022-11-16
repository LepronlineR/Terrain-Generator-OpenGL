#version 330 core

// layouts
layout (location = 0) in vec3 layoutPosition;
layout (location = 1) in vec3 layoutNormal;
layout (location = 2) in vec3 layoutColor;
layout (location = 3) in vec2 layoutTexture;


// Outputs (Note that these are in order of vertex | POS | NORM | COLOR | TEXUV )
out vec3 currentPos;
out vec3 normal;
out vec3 color;
out vec2 texturePos;

// uniform
uniform mat4 cameraMatrix;
uniform mat4 model;
uniform float size;

void main() {
	// out frag shader
	currentPos = vec3(model * vec4(layoutPosition, 1.0f));
	color = layoutColor;
	texturePos = layoutTexture;
	normal = layoutNormal;

	// return pos
	gl_Position = cameraMatrix * vec4(currentPos * size, 1.0f);
}