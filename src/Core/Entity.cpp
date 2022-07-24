#include "Entity.hpp"

Entity::Entity(const TexturedModel& model, const glm::vec3& position, const glm::vec3& rotation, float scale) : model(model), position(position), rotation(rotation), scale(scale) {

}

void Entity::IncreasePosition(const glm::vec3& p) {
	position += p;
}

void Entity::IncreaseRotation(const glm::vec3& r) {
	rotation += r;
}
