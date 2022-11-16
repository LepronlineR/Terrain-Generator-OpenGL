#ifndef __VBO_H__
#define __VBO_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Transform.h"
#include "Vertex.h"

class VBO {
public:
	VBO(Transform& transform);
	VBO(GLfloat vertices[]);
	VBO(std::vector<glm::mat4>& mats);
	VBO(std::vector<float> vertices);

	void Bind();
	void Unbind();
	void Delete();
private:
	GLuint ID;
};

#endif