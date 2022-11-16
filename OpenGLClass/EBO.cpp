#include "EBO.h"

EBO::EBO(Transform& transform) {
	glGenBuffers(1, &(EBO::ID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO::ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, transform.getElementBufferSize(), transform.getIndices().data(), GL_STATIC_DRAW);
}

EBO::EBO(GLuint indices[]) {
	glGenBuffers(1, &(EBO::ID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO::ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
}

EBO::EBO(std::vector<unsigned> indices) {
	glGenBuffers(1, &(EBO::ID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO::ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO::ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &(EBO::ID));
}