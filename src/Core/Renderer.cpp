#include "Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../Debug/Log.hpp"
#include "Camera.hpp"

Renderer::Renderer(const glm::uvec2& windowSize, const std::shared_ptr<Shaders>& shaders) : shaders(shaders), projectionMatrix(glm::perspective(FOV, (float)windowSize.x / (float)windowSize.y, NEAR_PLANE, FAR_PLANE)) {

}

void Renderer::Add(const Entity& entity) {
	auto& model = entity.GetModel();
	models[model].emplace_back(entity);
}

void Renderer::Render(const glm::mat4& viewMatrix) {
	shaders->SetMat4("vp", projectionMatrix * viewMatrix);

	for (auto& model : models) {
		auto& [key, value] = model;
		//key->BindVAO(true);
		for (auto& entity : value) {
			shaders->SetMat4("model", entity.GetModelMatrix());
			key->Draw();
		}
		//key->BindVAO(false);
	}

	models.clear();
}
