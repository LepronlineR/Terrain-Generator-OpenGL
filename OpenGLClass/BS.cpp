#include "BS.h"

BS::BS(glm::vec3 center, float radius) {
	BS::center = center;
	BS::radius = radius;
}

physicsdata BS::Intersected(BS other) {
	physicsdata data{};
	float d1 = BS::radius + other.getRadius();
	float d2 = glm::length(other.getCenter() - BS::center);
	data.distance = d2 - d1;
	data.intersected = d2 < d1;
	return data;
}

