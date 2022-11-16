#include "Transform.h"

Transform::Transform(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
	Transform::vertices = vertices;
	Transform::indices = indices;
	Transform::textures = textures;
}

Transform::Transform(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
	Transform::vertices = vertices;
	Transform::indices = indices;
}

GLsizeiptr Transform::getBufferSize() {
	return Transform::vertices.size() * sizeof(Vertex);
}

GLsizeiptr Transform::getElementBufferSize() {
	return Transform::indices.size() * sizeof(GLuint);
}

std::vector<Vertex> Transform::getVertices() {
	return vertices;
}

std::vector<GLuint> Transform::getIndices() {
	return indices;
}

std::vector<Texture> Transform::getTextures() {
	return textures;
}
