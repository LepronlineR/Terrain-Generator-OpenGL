#ifndef __EBO_H__
#define __EBO_H__

#include <glad/glad.h>
#include "Transform.h"

class EBO {
public:
	EBO(Transform& transform);
	EBO(GLuint indices[]);
	EBO(std::vector<unsigned> indices);

	void Bind();
	void Unbind();
	void Delete();

private:
	GLuint ID;

};

#endif