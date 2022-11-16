#ifndef __VAO_H__
#define __VAO_H__

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
	VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint space, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();

	GLuint getID();
private:
	GLuint ID;
};

#endif