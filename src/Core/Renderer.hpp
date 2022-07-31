#pragma once

#include "Entity.hpp"
#include "Shaders.hpp"

class Renderer {
public:
	Renderer(const glm::uvec2& windowSize, const std::shared_ptr<Shaders>& shaders);

	void Render(const Entity& entity, const glm::mat4& viewMatrix) const;

	const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }
private:
	constexpr static float FOV = 70.0f, NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;
	glm::mat4 projectionMatrix;
	const std::shared_ptr<Shaders> shaders;
};
