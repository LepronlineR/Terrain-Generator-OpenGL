#ifndef __TERRAIN_OPTIONS_H__
#define __TERRAIN_OPTIONS_H__

enum terrainAlgorithm {
	NONE, DIAMONDSQUARE, FRACTAL
};

typedef struct terrainOptions {
	float ySteep;
	float yHeight;
	int size;
	int minRandomAmount;
	int maxRandomAmount; 
	float lacunarity;
	float persistence;
	float frequency;
	int octaves;
	terrainAlgorithm algorithm;
};

#endif