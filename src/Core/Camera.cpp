#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Input.hpp"
#include "../Debug/Log.hpp"

void Camera::Update() {
	// input
	auto cameraSpeed = SPEED * Input::GetDeltaTime();
	if (Input::IsKeyHeld(Key::W)) {
		position += front * cameraSpeed;
	}
	if (Input::IsKeyHeld(Key::S)) {
		position -= front * cameraSpeed;
	}
	if (Input::IsKeyHeld(Key::A)) {
		position -= glm::normalize(glm::cross(front, UP)) * cameraSpeed;
	}
	if (Input::IsKeyHeld(Key::D)) {
		position += glm::normalize(glm::cross(front, UP)) * cameraSpeed;
	}

	viewMatrix = glm::lookAt(position, position + front, { 0, 1, 0 });
}

void Camera::CursorPositionCallback(float x, float y) {
	if (!cursorInit) {
		cursorInit = true;
		lastPosition = { x, y };
	}

	float xOffset = x - lastPosition.x;
	float yOffset = lastPosition.y - y;
	lastPosition = { x, y };

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch =  89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
}
