#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>
#include "terrainoptions.h"
#include "noise.h"

// ====================== TERRAIN ALGORITHMS ======================

// Diamond-Square algorithm (R) = random num
// [R] x [R]    [R]      x     [R]     [R]
//  x  x  x  ->  x (R combined) x   -> R w/ R comb  <--
// [R] x [R]	[R]      x     [R]     [R]
// diamond and square pattern (https://en.wikipedia.org/wiki/Diamond-square_algorithm)
// Note that size should be 2^n + 1
int** diamondSquareAlgorithm(int size, int minHeight, int maxHeight) {
	// initialize
	int** result = 0;
	result = new int* [size];
	for (int x = 0; x < size; x++) {
		result[x] = new int[size];
		for (int y = 0; y < size; y++) {
			result[x][y] = 1;
		}
	}
	
	// randomizer
	std::random_device rd;
	std::mt19937 seed(rd());
	std::uniform_int_distribution<int> randHeight(minHeight, maxHeight);
	
	// assign random edge
	result[0][0] = randHeight(seed);
	result[0][size - 1] = randHeight(seed);
	result[size - 1][0] = randHeight(seed);
	result[size - 1][size - 1] = randHeight(seed);

	int width = size - 1;

	while (width > 1) {
		int half = width / 2;

		// Diamond step 
		for (int x = 0; x < size - 1; x += width) {
			for (int y = 0; y < size - 1; y += width) {
				int diamond = result[x][y] + result[x + width][y] + 
					result[x][y + width] + result[x + width][y + width];
				diamond /= 4;
				diamond += randHeight(seed);
				result[x + half][y + half] = diamond;
			}
		}
		// Square step
		for (int x = 0; x < size - 1; x += half) {
			for (int y = ((x + half) % width); y < size - 1; y += width) {
				int square = result[(x - half + size - 1) % (size - 1)][y]
					+ result[(x + half) % (size - 1)][y]
					+ result[x][(y - half + size - 1) % (size - 1)]
					+ result[x][(y + half) % (size - 1)];
				square /= 4;
				square += randHeight(seed);
				result[x][y] = square;
				// wrap around
				if (x == 0) { result[size - 1][y] = square; }
				if (y == 0) { result[x][size - 1] = square; }
			}
		}
		// reset the randomness to lower
		minHeight /= 2;
		maxHeight /= 2;
		randHeight.param(decltype(randHeight)::param_type(minHeight, maxHeight));
		width /= 2;
	}
	return result;
}

// Fractal Noise
// simplex noise with adjustments 
int** fractalNoise(int size, int minHeight, int maxHeight, float lacunarity, float persistence, float frequency, int octaves) {
	// initialize
	Noise noise = Noise();
	int** result = 0;
	result = new int* [size];
	for (int x = 0; x < size; x++) {
		result[x] = new int[size];
		for (int y = 0; y < size; y++) {
			result[x][y] = 1;
		}
	}

	float amplitude = maxHeight / 2;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			float elevation = amplitude;
			float tFreq = frequency;
			float tAmp = amplitude;
			for (int z = 0; z < octaves; z++) {
				float sampx = tFreq * y;
				float sampy = tFreq * x;
				elevation += noise.simplexNoise(sampx, sampy) * tAmp;
				tFreq *= lacunarity;
				tAmp *= persistence;
			}
			// clamp
			float round = glm::round(elevation);
			// clamp values
			if (round > maxHeight)
				round = maxHeight;
			else if (round < minHeight)
				round = minHeight;
			result[x][y] = (int) round;
		}
	}
	return result;
}

VAO setupTerrain(const char* dir, unsigned int& totalStrips, unsigned int& totalTrianglesPerStrip,
	terrainOptions settings) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	std::vector<float> vertices;
	std::vector<unsigned> indices;

	// vertices

	int n = 10;
	int twon = glm::pow(2, n) + 1;
	int** result;
	unsigned char* map = NULL;

	switch (settings.algorithm) {
	case NONE:
		map = stbi_load(dir, &width, &height, &channels, 0);
		if (map) {
			std::cout << "TERRAIN: Loaded heightmap of size " << height << " x " << width << std::endl;
		} else {
			std::cout << "TERRAIN: Failed to load texture" << std::endl;
		}
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				int y = (int)*(map + (col + width * row) * channels);
				vertices.push_back(-height / 2.0f + row);
				vertices.push_back(y * settings.ySteep - settings.ySteep * settings.yHeight);
				vertices.push_back(-width / 2.0f + col);
			}
		}
		break;
	case DIAMONDSQUARE:
		result = diamondSquareAlgorithm(twon, settings.minRandomAmount, settings.maxRandomAmount);
		height = twon;
		width = twon;
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				vertices.push_back(-height / 2.0f + row);
				vertices.push_back(result[row][col] * settings.ySteep - settings.ySteep * settings.yHeight);
				vertices.push_back(-width / 2.0f + col);
			}
		}
		break;
	case FRACTAL:
		result = fractalNoise(twon, settings.minRandomAmount, settings.maxRandomAmount,
			settings.lacunarity, settings.persistence, settings.frequency, settings.octaves);
		height = twon;
		width = twon;
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				int y = result[row][col];
				vertices.push_back(-height / 2.0f + row);
				vertices.push_back(result[row][col]);
				vertices.push_back(-width / 2.0f + col);
			}
		}
		break;
	}
	std::cout << "TERRAIN: Loaded " << vertices.size() / 3 << " vertices" << std::endl;
	if(map == NULL)
		std::cout << "TERRAIN: Loaded heightmap of size " << height << " x " << width << std::endl;
	else
		stbi_image_free(map);
	// indices
	for (unsigned i = 0; i < height - 1; i += settings.size) {
		for (unsigned j = 0; j < width; j += settings.size) {
			for (unsigned k = 0; k < 2; k++) {
				indices.push_back(j + width * (i + k * settings.size));
			}
		}
	}
	std::cout << "TERRAIN: Loaded " << indices.size() << " indices" << std::endl;

	totalStrips = (height - 1)/settings.size;
	totalTrianglesPerStrip = width/settings.size * 2 - 2;
	std::cout << "TERRAIN: Created lattice of " << totalStrips << " strips with " << totalTrianglesPerStrip << " triangles each" << std::endl;
	std::cout << "TERRAIN: Generated " << totalTrianglesPerStrip * totalStrips << " triangles" << std::endl;

	// Generate Terrain
	VAO terrainVAO = VAO();
	terrainVAO.Bind();
	VBO terrainVBO(vertices);
	terrainVAO.LinkAttrib(terrainVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	EBO terrainEBO(indices);
	return terrainVAO;
}

void drawTerrain(Shader& shader, Camera& camera, VAO terrainVAO, Texture texture, unsigned int totalStrips, unsigned int totalTrianglesPerStrip) {
	shader.Activate();
	terrainVAO.Bind();
	unsigned int diffuse = 0;
	unsigned int specular = 0;
	unsigned int normal = 0;
	unsigned int height = 0;
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
	texture.Bind();
	texture.texUnit(shader, texturename.c_str(), 0);

	glm::vec3 cameraPosition = camera.getCamPos();
	glUniform3f(glGetUniformLocation(shader.getID(), "cameraPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
	camera.Matrix(shader, "cameraMatrix");
	// draw every triangle strip
	for (unsigned strip = 0; strip < totalStrips; strip++) {
		glDrawElements(GL_TRIANGLE_STRIP, totalTrianglesPerStrip + 2, GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned) * (totalTrianglesPerStrip + 2) * strip));
	}
}