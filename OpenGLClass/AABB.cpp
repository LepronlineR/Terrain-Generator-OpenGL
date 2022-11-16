#include "AABB.h"

AABB::AABB(glm::vec3 min, glm::vec3 max) {
	AABB::min = min;
	AABB::max = max;
}

physicsdata AABB::Intersected(AABB other) {
	physicsdata data;
	glm::vec3 d1 = other.getMin() - AABB::max;
	glm::vec3 d2 = AABB::min - other.getMax();
	glm::vec3 m = glm::max(d1, d2);
	data.distance = glm::length(m);
	data.intersected = data.distance < 0;
	return data;
}

