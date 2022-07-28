#include "Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../Debug/Log.hpp"
#include "Camera.hpp"

Renderer::Renderer(const glm::uvec2& windowSize, const std::shared_ptr<Shaders>& shaders) : shaders(shaders), projectionMatrix(glm::perspective(FOV, (float)windowSize.x / (float)windowSize.y, NEAR_PLANE, FAR_PLANE)) {
	shaders->Bind(true);
	shaders->SetInt("diffuse", 0);
}

void Renderer::Render(const Entity& entity, const glm::mat4& viewMatrix) const {
	auto& model = entity.GetModel();
	auto mvp = projectionMatrix * viewMatrix * entity.GetModelMatrix();
	shaders->SetMat4("mvp", mvp);
	model.Draw();
}
