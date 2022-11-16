#ifndef __MATH_HELPER__
#define __MATH_HELPER__

#include "PhysObj.h"

glm::vec3 integrateVelocity(PhysObj& object, float rate) {
	return object.getPos() + (object.getVelocity() * rate);
}

#endif