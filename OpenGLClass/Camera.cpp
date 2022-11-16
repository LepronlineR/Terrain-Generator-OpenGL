#include "Camera.h"

//		 y|				orientation: looking into
//		  |				up: y = 1.0f
//		 / \			
//	   x/	\z			

Camera::Camera(int width, int height, glm::vec3 pos) {
	Camera::orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::up_direction = glm::vec3(0.0f, 1.0f, 0.0f);

	Camera::cameraMatrix = glm::mat4(1.0f);
	Camera::width = width;
	Camera::height = height;
	Camera::position = pos;
}

glm::mat4 Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	glm::mat4 viewpoint = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	viewpoint = glm::lookAt(position, position + orientation, up_direction);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	return projection * viewpoint;
}

void Camera::updateCameraMatrix(float FOVdeg, float nearPlane, float farPlane) {
	cameraMatrix = updateMatrix(FOVdeg, nearPlane, farPlane);
}

void Camera::Matrix(Shader& shader, const char* uniform) {
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

glm::vec3 Camera::getCamPos() {
	return Camera::position;
}

glm::vec3 Camera::getCamOrientation() {
	return Camera::orientation;
}
glm::vec3 Camera::getCamUp() {
	return Camera::up_direction;
}

void Camera::Rotation() {

}

void Camera::Inputs(GLFWwindow* window, float rotationSpeed, float speed) {

	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += speed * -glm::normalize(glm::cross(orientation, up_direction));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		orientation = glm::rotate(orientation, glm::radians(rotationSpeed), up_direction);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * glm::normalize(glm::cross(orientation, up_direction));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		orientation = glm::rotate(orientation, glm::radians(-rotationSpeed), up_direction);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * up_direction;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += speed * -up_direction;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		position += speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		position += speed * -orientation;
	}

}