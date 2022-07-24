#pragma once

#include "Model.hpp"

class Entity {
public:
	Entity(const TexturedModel& model, const glm::vec3& position, const glm::vec3& rotation, float scale);

	void IncreasePosition(const glm::vec3& p);
	void IncreaseRotation(const glm::vec3& r);

	const TexturedModel& GetModel() const { return model; }
	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetRotation() const { return rotation; }
	float GetScale() const { return scale; }
private:
	TexturedModel model;
	glm::vec3 position, rotation;
	float scale;
};
