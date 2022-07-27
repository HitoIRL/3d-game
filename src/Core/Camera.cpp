#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Input.hpp"

constexpr float CAMERA_SPEED = 2.5f;
constexpr glm::vec3 UP_VECTOR = { 0, 1, 0 };

Camera::Camera() : front(0, 0, -1), right(1, 0, 0), position(0), pitch(0), yaw(-90) {
	Input::AddCursorPanCallback(std::bind(&Camera::CursorPanCallback, this, std::placeholders::_1));
}

void Camera::Update() {
	auto velocity = CAMERA_SPEED * Input::GetDeltaTime();
	if (Input::IsKeyHeld(Key::W))
		position += front * velocity;
	if (Input::IsKeyHeld(Key::S))
		position -= front * velocity;
	if (Input::IsKeyHeld(Key::A))
		position -= right * velocity;
	if (Input::IsKeyHeld(Key::D))
		position += right * velocity;

	viewMatrix = glm::lookAt(position, position + front, { 0, 1, 0 });
}

void Camera::CursorPanCallback(const glm::vec2& pos) {
	static bool cursorInit = false;
	if (!cursorInit) {
		cursorInit = true;
		lastPosition = pos;
	}

	float xOffset = pos.x - lastPosition.x;
	float yOffset = lastPosition.y - pos.y;
	lastPosition = pos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch =  89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction = {
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	};
	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, UP_VECTOR));
}
