#include "Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "../Debug/Log.hpp"

Renderer::Renderer(const glm::uvec2& windowSize, const std::shared_ptr<Shaders>& shaders) : shaders(shaders), projectionMatrix(glm::perspective(FOV, (float)windowSize.x / (float)windowSize.y, NEAR_PLANE, FAR_PLANE)) {

}

void Renderer::Add(const Entity& entity) {
	auto& model = entity.GetModel();
	models[model].emplace_back(entity);
}

void Renderer::Add(const Terrain& terrain) {
	auto& rawModel = terrain.GetRawModel();
	terrains[rawModel].emplace_back(terrain);
}

void Renderer::Render(const glm::mat4& viewMatrix) {
	shaders->SetMat4("vp", projectionMatrix * viewMatrix);

	for (auto& model : models) {
		auto& [key, value] = model;
		auto& rawModel = key->GetRawModel();
		rawModel->BindVAO(true);
		for (auto& entity : value) {
			shaders->SetMat4("model", entity.GetModelMatrix());
			key->Draw();
		}
		rawModel->BindVAO(false);
	}
	models.clear();

	for (auto& terrain : terrains) {
		auto& [key, value] = terrain;
		key->BindVAO(true);
		shaders->SetMat4("model", glm::mat4(1));
		for (auto& entity : value)
			key->Draw();
		key->BindVAO(false);
	}
	terrains.clear();
}
