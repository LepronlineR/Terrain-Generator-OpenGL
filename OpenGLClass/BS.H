#ifndef __BOUNDING_SPHERE__
#define __BOUNDING_SPHERE__

#include <glm/glm.hpp>
#include "physicsdata.h"

class BS {
public:
	BS(glm::vec3 center, float radius);

	physicsdata Intersected(BS other);

	glm::vec3 getCenter() { return BS::center; }
	float getRadius() { return BS::radius; }

private:
	glm::vec3 center;
	float radius;
};

#endif