#include "Mesh.h"

Mesh::Mesh(Transform& transform, std::vector<glm::mat4> instances, GLsizei instance_count) {
	Mesh::transform = transform;
	Mesh::instances = instance_count;
	setupMesh(instances);
}

Mesh::Mesh(Transform& transform) {
	Mesh::transform = transform;
	Mesh::instances = 1;
	std::vector<glm::mat4> fake;
	setupMesh(fake);
}

void Mesh::setupMesh(std::vector<glm::mat4> inst) {
	Mesh::VAO.Bind();
	VBO IVBO(inst);
	VBO VBO(Mesh::transform);
	EBO EBO(Mesh::transform);

	Mesh::VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	Mesh::VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	Mesh::VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	Mesh::VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	if (Mesh::instances > 1) {
		IVBO.Bind();
		Mesh::VAO.LinkAttrib(IVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
		Mesh::VAO.LinkAttrib(IVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		Mesh::VAO.LinkAttrib(IVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		Mesh::VAO.LinkAttrib(IVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}

	Mesh::VAO.Unbind();
	IVBO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera) {
	shader.Activate();
	Mesh::VAO.Bind();
	unsigned int diffuse = 0;
	unsigned int specular = 0;
	unsigned int normal = 0;
	unsigned int height = 0;
	int count = 0;
	for (Texture texture : Mesh::transform.getTextures()) {
		std::string texturename;
		switch (texture.getType()) {
		case DIFFUSE:
			texturename = "diffuse" + std::to_string(diffuse++);
			break;
		case SPECULAR:
			texturename = "specular" + std::to_string(specular++);
			break;
		case NORMAL:
			texturename = "normal" + std::to_string(normal++);
			break;
		case HEIGHT:
			texturename = "height" + std::to_string(height++);
			break;
		case DEFAULT:
			break;
		}
		texture.texUnit(shader, texturename.c_str(), count);
		texture.Bind();
		count++;
	}
	glm::vec3 cameraPosition = camera.getCamPos();
	glUniform3f(glGetUniformLocation(shader.getID(), "cameraPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
	camera.Matrix(shader, "cameraMatrix");
	// instancing
	if (Mesh::instances > 1) {
		glDrawElementsInstanced(GL_TRIANGLES, transform.getIndices().size(), GL_UNSIGNED_INT, 0, Mesh::instances);
	} else {
		// Draw the actual mesh
		glDrawElements(GL_TRIANGLES, transform.getIndices().size(), GL_UNSIGNED_INT, 0);
	}
}