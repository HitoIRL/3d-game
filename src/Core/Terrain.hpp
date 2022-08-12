#pragma once

#include <glm/vec2.hpp>

#include "Model.hpp"

class Terrain {
public:
	Terrain(const glm::uvec2& grid, const std::shared_ptr<Texture>& texture);

	const std::shared_ptr<RawModel>& GetRawModel() const { return rawModel; }
private:
	std::shared_ptr<RawModel> rawModel;
};
