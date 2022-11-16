#include "Vertex.h"

Vertex::Vertex() {
	Vertex::position = glm::vec3(0.0f, 0.0f, 0.0f);
	Vertex::normal = glm::vec3(0.0f, 0.0f, 0.0f);
	Vertex::color = glm::vec3(0.0f, 0.0f, 0.0f);
	Vertex::textureUV = glm::vec2(0.0f, 0.0f);
}

Vertex::Vertex(glm::vec3 position) {
	Vertex::position = position;
	Vertex::normal = glm::vec3(0.0f, 0.0f, 0.0f);
	Vertex::color = glm::vec3(0.0f, 0.0f, 0.0f);
	Vertex::textureUV = glm::vec2(0.0f, 0.0f);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal) {
	Vertex::position = position;
	Vertex::normal = normal;
	Vertex::color = glm::vec3(0.0f, 0.0f, 0.0f);
	Vertex::textureUV = glm::vec2(0.0f, 0.0f);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color) {
	Vertex::position = position;
	Vertex::normal = normal;
	Vertex::color = color;
	Vertex::textureUV = glm::vec2(0.0f, 0.0f);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 textureUV) {
	Vertex::position = position;
	Vertex::normal = normal;
	Vertex::color = color;
	Vertex::textureUV = textureUV;
}

glm::vec3 Vertex::getPos() { return Vertex::position; }
glm::vec3 Vertex::getNormal() { return Vertex::normal; }
glm::vec3 Vertex::getColor() { return Vertex::color; }
glm::vec2 Vertex::getTextUV() { return Vertex::textureUV; }

void Vertex::setPos(glm::vec3 pos) { Vertex::position = pos; }
void Vertex::setNormal(glm::vec3 normal) { Vertex::normal = normal; }
void Vertex::setColor(glm::vec3 color) { Vertex::color = color; }
void Vertex::setTextUV(glm::vec3 textureUV) { Vertex::textureUV = textureUV; }