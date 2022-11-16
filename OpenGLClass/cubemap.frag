#version 330 core

out vec4 FragColor;

in vec3 textureUV;

uniform samplerCube cubemap;

void main(){
	FragColor = texture(cubemap, textureUV);
}