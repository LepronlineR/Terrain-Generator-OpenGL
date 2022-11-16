#ifndef __VERTEX_H_
#define __VERTEX_H_

#include <glm/glm.hpp>

// vertex contains position, normal, color, texture UV

class Vertex {
public:
	// constructors
	Vertex();
	Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 texture);

	// get
	glm::vec3 getPos();
	glm::vec3 getNormal();
	glm::vec3 getColor();
	glm::vec2 getTextUV();

	// set
	void setPos(glm::vec3 pos);
	void setNormal(glm::vec3 normal);
	void setColor(glm::vec3 color);
	void setTextUV(glm::vec3 textureUV);

private:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 textureUV;
};

#endif