#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Texture.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
};

class RawModel {
public:
	RawModel(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
	~RawModel();

	void BindVAO(bool state) const;

	std::uint32_t GetIndexCount() const { return indexCount; }
	std::uint32_t GetIndicesOffset() const { return indicesOffset;}
private:
	std::uint32_t vao, buffer;
	std::uint32_t indexCount, indicesOffset;
};

class TexturedModel {
public:
	TexturedModel(const RawModel& rawModel, const Texture& texture);

	void Draw() const;
	void BindTexture() const;

	const RawModel& GetRawModel() const { return rawModel; }
private:
	RawModel rawModel;
	Texture texture;
};
