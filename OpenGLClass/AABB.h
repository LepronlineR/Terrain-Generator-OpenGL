#ifndef __AXIS_ALIGNED_BOUNDING_BOXES__
#define __AXIS_ALIGNED_BOUNDING_BOXES__

#include <glm/glm.hpp>
#include "physicsdata.h"

class AABB {
public:
	AABB(glm::vec3 min, glm::vec3 max);

	physicsdata Intersected(AABB other);

	glm::vec3 getMin() { return AABB::min; };
	glm::vec3 getMax() { return AABB::max; };


private:
	glm::vec3 min;
	glm::vec3 max;
};

#endif