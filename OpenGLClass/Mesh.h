#ifndef __MESH__H__
#define __MESH__H__

#include <string>
#include <vector>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh {
public:
	// constructor
	Mesh(Transform& transform, std::vector<glm::mat4> instances, GLsizei instance_count);
	Mesh(Transform& transform);

	// draw mesh
	void Draw(Shader& shader, Camera& camera);

private:
	VAO VAO;
	Transform transform;
	GLsizei instances;
	void setupMesh(std::vector<glm::mat4> inst);
};

#endif