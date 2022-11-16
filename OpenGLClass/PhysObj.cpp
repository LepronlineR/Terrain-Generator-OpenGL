#include "PhysObj.h"

PhysObj::PhysObj(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration) {
	PhysObj::position = position;
	PhysObj::velocity = velocity;
	PhysObj::acceleration = acceleration;
}