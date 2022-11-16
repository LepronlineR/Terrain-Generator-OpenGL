#include "VBO.h"

VBO::VBO(Transform& transform) {
	glGenBuffers(1, &(VBO::ID));
	glBindBuffer(GL_ARRAY_BUFFER, VBO::ID);
	glBufferData(GL_ARRAY_BUFFER, transform.getBufferSize(), transform.getVertices().data(), GL_STATIC_DRAW);
}

VBO::VBO(GLfloat vertices[]) {
	glGenBuffers(1, &(VBO::ID));
	glBindBuffer(GL_ARRAY_BUFFER, VBO::ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
}

VBO::VBO(std::vector<float> vertices) {
	glGenBuffers(1, &(VBO::ID));
	glBindBuffer(GL_ARRAY_BUFFER, VBO::ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
}

VBO::VBO(std::vector<glm::mat4>& mats) {
	glGenBuffers(1, &(VBO::ID));
	glBindBuffer(GL_ARRAY_BUFFER, VBO::ID);
	glBufferData(GL_ARRAY_BUFFER, mats.size() * sizeof(glm::mat4), mats.data(), GL_STATIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO::ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &(VBO::ID));
}