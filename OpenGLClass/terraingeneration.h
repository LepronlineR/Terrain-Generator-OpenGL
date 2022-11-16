#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

class Terrain {
public:
	Terrain(const char* heightmap);
	void Draw(Shader& shader, Camera& camera);
	void Bind();

	int getTotalStrips() { return totalStrips; };
	int getTotalTriangles() { return totalTrianglesPerStrip; }

private:
	VAO terrainVAO;
	int width;
	int height;
	int channels;
	std::vector<float> vertices;
	std::vector<unsigned> indices;
	int totalStrips;
	int totalTrianglesPerStrip;
};

#endif