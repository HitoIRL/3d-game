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

Renderer::Renderer(const glm::uvec2& windowSize, const std::shared_ptr<Shaders>& shaders) : shaders(shaders), projection(0) {
	projection = glm::perspective(FOV, (float)windowSize.x / (float)windowSize.y, NEAR_PLANE, FAR_PLANE);
	shaders->Bind(true);
	shaders->SetMat4("_projection", projection);
}

void Renderer::Render(const Entity& entity) const {
	auto& model = entity.GetModel();
	auto& rawModel = model.GetRawModel();
	auto transformation = CreateTransformationMatrix(entity.GetPosition(), entity.GetRotation(), entity.GetScale());
	shaders->SetMat4("_transformation", transformation);
	rawModel.BindVAO(true);
	model.BindTexture();
	shaders->SetInt("_texture", 0);
	model.Draw();
	rawModel.BindVAO(false);
}
