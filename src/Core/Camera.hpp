#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x2.hpp>

class Camera {
public:
	Camera();

	void Update();

	const glm::mat4& GetViewMatrix() const { return viewMatrix; }
	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetFront() const { return front; } // aka direction
private:
	void CursorPanCallback(const glm::vec2& pos);

	glm::vec3 front, right, position;
	float pitch, yaw;
	glm::mat4 viewMatrix;
	glm::vec2 lastPosition; // cursor
};
