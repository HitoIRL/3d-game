#pragma once

#include <glm/mat4x2.hpp>

#include "Model.hpp"

class Entity {
public:
	Entity(const std::shared_ptr<Model>& model, const glm::vec3& position, const glm::vec3& rotation, float scale);

	void IncreasePosition(const glm::vec3& p);
	void IncreaseRotation(const glm::vec3& r);

	const std::shared_ptr<Model>& GetModel() const { return model; }
	glm::mat4 GetModelMatrix() const; // needs to be calculated
private:
	const std::shared_ptr<Model> model;
	glm::vec3 position, rotation;
	float scale;
};
