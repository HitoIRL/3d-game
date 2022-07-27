#include "Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../Debug/Log.hpp"
#include "Camera.hpp"

glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, float scale) { // todo: move to entity class
	glm::mat4 temp(1);
	temp = glm::translate(temp, translation);
	temp = glm::rotate(temp, glm::radians(rotation.x), { 1, 0, 0 });
	temp = glm::rotate(temp, glm::radians(rotation.y), { 0, 1, 0 });
	temp = glm::rotate(temp, glm::radians(rotation.z), { 0, 0, 1 });
	temp = glm::scale(temp, glm::vec3(scale));
	return temp;
}

Renderer::Renderer(const glm::uvec2& windowSize, const std::shared_ptr<Shaders>& shaders) : shaders(shaders), projectionMatrix(glm::perspective(FOV, (float)windowSize.x / (float)windowSize.y, NEAR_PLANE, FAR_PLANE)) {

}

void Renderer::Render(const Entity& entity, const glm::mat4& viewMatrix) const {
	auto& model = entity.GetModel();
	auto& rawModel = model.GetRawModel();
	auto transformation = CreateTransformationMatrix(entity.GetPosition(), entity.GetRotation(), entity.GetScale());
	auto mvp = projectionMatrix * viewMatrix * transformation;
	shaders->SetMat4("mvp", mvp);
	rawModel.BindVAO(true);
	model.BindTexture();
	shaders->SetInt("_texture", 0);
	model.Draw();
	rawModel.BindVAO(false);
}
