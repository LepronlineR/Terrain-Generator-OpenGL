#include "plane.h"

physicsdata Plane::Intersect(BS other) {
	physicsdata data{};
	float distanceSphere = glm::abs(glm::dot(Plane::normal, other.getCenter())) + Plane::magnitude - other.getRadius();
	data.intersected = distanceSphere < 0;
	data.distance = distanceSphere;
	return data;
}

Plane Plane::Normalized() {
	return Plane(Plane::normal / glm::length(Plane::normal), magnitude / glm::length(Plane::normal));
}