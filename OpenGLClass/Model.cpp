#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"


Model::Model(std::string file, std::string directory, GLuint instance_count, std::vector<glm::mat4> instances) {
    Model::directory = directory;
    Model::instances = instances;
    Model::instance_count = instance_count;
	load(directory + file);
}

Model::Model(std::string file, std::string directory) {
    Model::directory = directory;
    std::vector<glm::mat4> fake;
    Model::instances = fake;
    Model::instance_count = 1;
    load(directory + file);
}

void Model::load(std::string file) {
    std::cout << "Loading in Model from file: " << file << std::endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ASSIMP Loading Error: \n" << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::Draw(Shader& shader, Camera& camera) {
    for (Mesh mesh : Model::meshes) {
        mesh.Draw(shader, camera);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // process the mesh for each node
    for (int x = 0; x < node->mNumMeshes; x++) {
        aiMesh* temp = scene->mMeshes[node->mMeshes[x]];
        meshes.push_back(processMesh(temp, scene));
    }
    // process the mesh for the children of the node
    for (int x = 0; x < node->mNumChildren; x++) {
        processNode(node->mChildren[x], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures_;

    // vertices
    for (int x = 0; x < mesh->mNumVertices; x++) {
        // position
        glm::vec3 position(mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z);
        // normal
        glm::vec3 normal(0.0f, 0.0f, 0.0f);
        if (mesh->HasNormals())
            normal = glm::vec3(mesh->mNormals[x].x, mesh->mNormals[x].y, mesh->mNormals[x].z);
        // color
        glm::vec3 color(0.0f, 0.0f, 0.0f);
        if (mesh->mColors[0])
            color = glm::vec3(mesh->mColors[0][x].r, mesh->mColors[0][x].g, mesh->mColors[0][x].b);
        // texture UV
        glm::vec2 texture(0.0f, 0.0f);
        if (mesh->mTextureCoords[0])
            texture = glm::vec2(mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y);
        // add
        vertices.push_back(Vertex(position, normal, color, texture));
    }
    // indices
    for (int x = 0; x < mesh->mNumFaces; x++) {
        for (int y = 0; y < mesh->mFaces[x].mNumIndices; y++)
            indices.push_back(mesh->mFaces[x].mIndices[y]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // textures
    std::vector<Texture> diffuses = loadTextures(material, aiTextureType_DIFFUSE, DIFFUSE);
    std::vector<Texture> speculars = loadTextures(material, aiTextureType_SPECULAR, SPECULAR);
    std::vector<Texture> normals = loadTextures(material, aiTextureType_HEIGHT, NORMAL);
    std::vector<Texture> heights = loadTextures(material, aiTextureType_AMBIENT, HEIGHT);
    std::vector<Texture> combined[] = { diffuses, speculars, normals, heights };
    textures_ = combineTextures(combined, 4);

    Transform temp(vertices, indices, textures_);
    return Mesh(temp, instances, instance_count);
}

std::vector<Texture> Model::combineTextures(std::vector<Texture> textures[], int amount) {
    std::vector<Texture> texture;
    for (int x = 0; x < amount; x++) {
        for (Texture t : textures[x]) {
            texture.push_back(t);
        }
    }
    return texture;
}

std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType type, TextureType typeName) {
    std::vector<Texture> other;
    for (int x = 0; x < material->GetTextureCount(type); x++) {
        aiString str;
        material->GetTexture(type, x, &str);
        bool skip = false;
        for (int y = 0; y < textures.size(); y++) {
            std::string comp = directory + str.C_Str();
            if (std::strcmp(textures[y].getPath().data(), comp.c_str()) == 0){
                other.push_back(textures[y]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip){   // if texture hasn't been loaded already, load it
            std::string location = Model::directory + str.C_Str();
            Texture texture(location.c_str(), typeName, Model::textures.size());
            std::cout << "LOADED: " << location << " " << Model::textures.size() << std::endl;
            other.push_back(texture);
            Model::textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return other;
}