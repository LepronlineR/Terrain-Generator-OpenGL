#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &(VAO::ID));
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint space, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, space, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(VAO::ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &(VAO::ID));
}

GLuint VAO::getID() {
	return VAO::ID;
}

/*
// first, configure the cube's VAO (and terrainVBO + terrainIBO)
	//VAO terrainVAO = VAO();	// glGenVertexArrays(1, &terrainVAO);
	//terrainVAO.Bind();		// glBindVertexArray(terrainVAO);

	//VBO terrainVBO(vertices); // glGenBuffers(1, &terrainVBO);
							  // glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
							  // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//terrainVAO.LinkAttrib(terrainVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
								// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
								// glEnableVertexAttribArray(0);
	//EBO terrainEBO(indices);
							// glGenBuffers(1, &terrainIBO);
							// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
							// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
*/