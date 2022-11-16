#ifndef __CAMERA_H__
#define __CAMERA_H__

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera {
public:
	// Constructor
	Camera(int width, int height, glm::vec3 position);
	
	// Matrix operations
	void updateCameraMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);

	// Observers
	glm::vec3 getCamPos();
	glm::vec3 getCamOrientation();
	glm::vec3 getCamUp();

	// Interaction
	void Rotation();
	void Inputs(GLFWwindow* window, float rotationSpeed, float speed);

private:
	int width;
	int height;
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up_direction;
	glm::mat4 cameraMatrix;

	glm::mat4 updateMatrix(float FOVdeg, float nearPlane, float farPlane);
};
#endif