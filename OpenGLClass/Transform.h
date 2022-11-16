#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include<vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"
#include "Texture.h"

class Transform {
public:
	// constructors
	Transform(){};
	Transform(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	Transform(std::vector<Vertex> vertices, std::vector<GLuint> indices);

	GLsizeiptr getBufferSize();
	GLsizeiptr getElementBufferSize();

	// getters
	std::vector<Vertex> getVertices();
	std::vector<GLuint> getIndices();
	std::vector<Texture> getTextures();


private: 
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
};

#endif
