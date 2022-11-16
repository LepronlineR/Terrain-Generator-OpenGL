#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Transform.h"
#include "Vertex.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "cube.h"
#include "Shader.h"

class Framebuffer {
public:
	Framebuffer(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);

	void Initialize();
	void Bind();
	void Draw();

	Shader getShader();

private:
	GLuint ID;
	GLuint Texture;
	GLuint RBO;
	Shader FBShader;
	Transform transform;

	VAO VAO;
};

#endif