#include <glm/glm.hpp>
#include "noise.h"

Noise::Noise() {
	for (int i = 0; i < 512; i++) 
		perm[i] = p[i & 255];
}

// Simplex Noise (implementation from here)
// https://weber.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
double Noise::simplexNoise(int x, int y) {
	double n0, n1, n2;

	// Skew the input space to determine which simplex cell we're in
	double F2 = 0.5 * (glm::sqrt(3.0) - 1.0);
	double s = (x + y) * F2; // Hairy factor for 2D
	int i = fastfloor(x + s);
	int j = fastfloor(y + s);

	double G2 = (3.0 - glm::sqrt(3.0)) / 6.0;
	double t = (i + j) * G2;
	double X0 = i - t; // Unskew the cell origin back to (x,y) space
	double Y0 = j - t;
	double x0 = x - X0; // The x,y distances from the cell origin
	double y0 = y - Y0;

	// Determine which simplex we are in.
	int i1, j1; 
	if (x0 > y0) {
		i1 = 1; j1 = 0; 
	} else { 
		i1 = 0; j1 = 1; 
	}

	double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
	double y1 = y0 - j1 + G2;
	double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
	double y2 = y0 - 1.0 + 2.0 * G2;

	int ii = i & 255;
	int jj = j & 255;
	int gi0 = perm[ii + perm[jj]] % 12;
	int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
	int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;

	// Calculate the contribution from the three corners
	double t0 = 0.5 - x0 * x0 - y0 * y0;
	if (t0 < 0) n0 = 0.0;
	else {
		t0 *= t0;
		n0 = t0 * t0 * dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
	}
	double t1 = 0.5 - x1 * x1 - y1 * y1;
	if (t1 < 0) n1 = 0.0;
	else {
		t1 *= t1;
		n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
	}

	double t2 = 0.5 - x2 * x2 - y2 * y2;
	if (t2 < 0) n2 = 0.0;
	else {
		t2 *= t2;
		n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
	}
	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 70.0 * (n0 + n1 + n2);
}