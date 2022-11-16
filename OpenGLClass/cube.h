#ifndef __CUBE_H__
#define __CUBE_H__

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"

class Cube {
public:
	Cube() {}
	std::vector<Vertex> cubeVertices();
	std::vector<GLuint> cubeIndices();

};

#endif