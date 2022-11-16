#ifndef __MODEL_H__
#define __MODEL_H__

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/matrix.hpp>

#include "Transform.h"
#include "VAO.h"
#include "Camera.h"
#include "Mesh.h"

class Model {
public:
	// Constructor
	Model(std::string file, std::string dir, GLuint instance_count, std::vector<glm::mat4> instances);
	Model(std::string file, std::string dir);

	// Draw the model
	void Draw(Shader& shader, Camera& camera);

private:
	std::vector<Texture> textures;
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<glm::mat4> instances;
	GLuint instance_count;

	// loads a model
	void load(std::string file);

	// processes an aiNode node
	void processNode(aiNode* node, const aiScene* scene);
	
	// processes an aiMesh mesh
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	
	// load in material
	std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, TextureType typeName);

	// append texture vector[] to single texture vector
	std::vector<Texture> combineTextures(std::vector<Texture> textures[], int amount);
};

#endif