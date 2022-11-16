#include "cube.h"



std::vector<Vertex> Cube::cubeVertices() {
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(glm::vec3(-0.1f, -0.1f, 0.1f)));
	vertices.push_back(Vertex(glm::vec3(-0.1f, -0.1f, -0.1f)));
	vertices.push_back(Vertex(glm::vec3(0.1f, -0.1f, -0.1f)));
	vertices.push_back(Vertex(glm::vec3(0.1f, -0.1f, 0.1f)));
	vertices.push_back(Vertex(glm::vec3(-0.1f, 0.1f, 0.1f)));
	vertices.push_back(Vertex(glm::vec3(-0.1f, 0.1f, -0.1f)));
	vertices.push_back(Vertex(glm::vec3(0.1f, 0.1f, -0.1f)));
	vertices.push_back(Vertex(glm::vec3(0.1f, 0.1f, 0.1f)));
	return vertices;
}

std::vector<GLuint> Cube::cubeIndices() {
	GLuint cubeIndex[] = {
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};
	std::vector<GLuint> indices(cubeIndex, cubeIndex + sizeof(cubeIndex) / sizeof(GLuint));
	return indices;
}