#ifndef __PHYSICS_OBJECT_H__
#define __PHYSICS_OBJECT_H__

#include <glm/glm.hpp>
// #include "mathhelper.h"

class PhysObj {
public:
	PhysObj(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration);

	glm::vec3 getPos() { return position; }
	glm::vec3 getVelocity() { return velocity; }
	glm::vec3 getAcceleration() { return acceleration; }

	void move(float rate) {
		PhysObj::position += (PhysObj::velocity * rate);
	}

private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
};
#endif 