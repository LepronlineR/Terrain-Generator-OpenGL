#version 330 core

// Outputs
out vec4 FragColor;

// Inputs (Note that these are in order of vertex | POS | NORM | COLOR | TEXUV )
in vec3 currentPos;
in vec3 normal;
in vec3 color;
in vec2 texturePos;

// Uniform
uniform sampler2D screen;


void main(){
	FragColor = texture(screen, texturePos);
}