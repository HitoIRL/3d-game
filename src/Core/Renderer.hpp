#pragma once

#include "Shaders.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"

class Renderer {
public:
	Renderer(const glm::uvec2& windowSize, const std::shared_ptr<Shaders>& shaders);

	void Add(const Entity& entity);
	void Add(const Terrain& terrain);
	void Render(const glm::mat4& viewMatrix);
private:
	constexpr static float FOV = 70.0f, NEAR_PLANE = 0.1f, FAR_PLANE = 100.0f;
	glm::mat4 projectionMatrix;
	const std::shared_ptr<Shaders> shaders;
	std::unordered_map<std::shared_ptr<Model>, std::vector<Entity>> models;
	std::unordered_map<std::shared_ptr<RawModel>, std::vector<Terrain>> terrains;
};
