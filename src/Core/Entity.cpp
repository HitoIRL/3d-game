#include "Entity.hpp"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(const std::shared_ptr<Model>& model, const glm::vec3& position, const glm::vec3& rotation, float scale) : model(model), position(position), rotation(rotation), scale(scale) {

}

void Entity::IncreasePosition(const glm::vec3& p) {
	position += p;
}

void Entity::IncreaseRotation(const glm::vec3& r) {
	rotation += r;
}

glm::mat4 Entity::GetModelMatrix() const {
	glm::mat4 temp(1);
	temp = glm::translate(temp, position);
	temp = glm::rotate(temp, glm::radians(rotation.x), { 1, 0, 0 });
	temp = glm::rotate(temp, glm::radians(rotation.y), { 0, 1, 0 });
	temp = glm::rotate(temp, glm::radians(rotation.z), { 0, 0, 1 });
	temp = glm::scale(temp, glm::vec3(scale));
	return temp;
}
