#ifndef __PLANE_H__
#define __PLANE_H__

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"
#include "physicsdata.h"
#include "BS.H"
#include "AABB.h"

class Plane {
public:
	Plane(glm::vec3 normal, float magnitude) {
		Plane::normal = normal;
		Plane::magnitude = magnitude;
	};

	Plane Normalized();
	physicsdata Intersect(BS other);

private:
	glm::vec3 normal;
	float magnitude;
};

#endif