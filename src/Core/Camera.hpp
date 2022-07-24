#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x2.hpp>

class Camera {
public:
	void Update();

	void CursorPositionCallback(float x, float y);

	const glm::vec3& GetPosition() const { return position; }
	float GetPitch() const { return pitch; }
	float GetYaw() const { return yaw; }
	const glm::mat4& GetViewMatrix() const { return viewMatrix; }
private:
	static constexpr float SPEED = 2.5f;
	static constexpr glm::vec3 UP = { 0, 1, 0 };
	glm::vec3 front, position, target;
	float pitch, yaw = -90.0f;
	glm::mat4 viewMatrix;
	bool cursorInit;
	glm::vec2 lastPosition; // cursor
};
