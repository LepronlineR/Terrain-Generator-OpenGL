#include "terraingeneration.h"

Terrain::Terrain(const char* heightmap) {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* map = stbi_load(heightmap, &width, &height, &channels, 0);
	if (map){
		std::cout << "TERRAIN: Loaded heightmap of size " << height << " x " << width << std::endl;
	} else {
		std::cout << "TERRAIN: Failed to load texture" << std::endl;
	}

	// vertices
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	unsigned bytePerPixel = channels;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			unsigned char* pixelOffset = map + (j + width * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
		}
	}
	std::cout << "TERRAIN: Loaded " << vertices.size() / 3 << " vertices" << std::endl;
	stbi_image_free(map);

	// indices
	for (unsigned i = 0; i < height - 1; i ++){
		for (unsigned j = 0; j < width; j ++){
			for (unsigned k = 0; k < 2; k++){
				indices.push_back(j + width * (i + k));
			}
		}
	}
	std::cout << "TERRAIN: Loaded " << indices.size() << " indices" << std::endl;

	Terrain::totalStrips = (height - 1);
	Terrain::totalTrianglesPerStrip = width * 2 - 2;
	std::cout << "TERRAIN: Created lattice of " << totalStrips << " strips with " << totalTrianglesPerStrip << " triangles each" << std::endl;
	std::cout << "TERRAIN: Generated " << totalTrianglesPerStrip * totalStrips << " triangles" << std::endl;

	// Generate Terrain
	VAO terrainVAO = VAO();
	terrainVAO.Bind();
	VBO terrainVBO(vertices);
	terrainVAO.LinkAttrib(terrainVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	EBO terrainEBO(Terrain::indices);
}

void Terrain::Draw(Shader& shader, Camera& camera) {
	terrainVAO.Bind();
	
	for (unsigned strip = 0; strip < totalStrips; strip++){
		glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
			totalTrianglesPerStrip + 2,   // number of indices to render
			GL_UNSIGNED_INT,     // index data type
			(void*)(sizeof(unsigned) * (totalTrianglesPerStrip + 2) * strip)); // offset to starting index
	}
}

void Terrain::Bind() {
	terrainVAO.Bind();
}