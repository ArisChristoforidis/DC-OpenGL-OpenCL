#include "Camera.h"

Camera* Camera::instance = NULL;

Camera* Camera::getInstance() {
	if (instance == NULL) {
		instance = new Camera();
	}

	return instance;
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;

	this->front = glm::vec3(0.0f,0.0f,-1.0f);
	this->movementSpeed = SPEED*0.1f;
	this->mouseSensitivity = SENSITIVITY;

	UpdateCamera();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboardMovement(CameraMovement dir, double deltaTime) {
	float velocity = movementSpeed * deltaTime;
	
	if(dir == FORWARD)position += front * velocity;

	if(dir == BACKWARD)position += -front * velocity;

	if(dir == RIGHT)position += right * velocity;

	if(dir == LEFT)position += -right * velocity;
}

void Camera::ProcessMouseLook(float xOffset, float yOffset, GLboolean constrainPitch) {
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw = std::fmod((yaw + xOffset),(GLfloat)360.0f);
	pitch += yOffset;

	if(constrainPitch){
		if(pitch > 89.0f) pitch = 89.0f;
		if(pitch < -89.0f) pitch = -89.0f;
	}

	UpdateCamera();
}

void Camera::UpdateCamera() {
	glm::vec3 updatedFrontVector;

	updatedFrontVector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	updatedFrontVector.y = sin(glm::radians(pitch));
	updatedFrontVector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(updatedFrontVector);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
