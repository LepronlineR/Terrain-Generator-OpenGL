#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Transform.h"
#include "Camera.h"
#include "Shader.h"

class Cubemap {
public:

	Cubemap(std::string directories[], const unsigned int width, const unsigned int height);

	// draw
	void Draw(Shader& shader, Camera& camera);

	GLuint getID();
	void bindVAO();
	void activate(Shader& shader);

private:
	VAO VAO;
	GLuint ID;
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;
	unsigned int skyboxVAO;

	void load(std::vector<std::string> faces);
	void generateTexture();
};

#endif