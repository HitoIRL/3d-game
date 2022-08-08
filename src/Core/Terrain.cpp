#include "Terrain.hpp"

#include "../Debug/Log.hpp"

constexpr std::uint32_t VertexCount = 128;
constexpr std::uint32_t Size = 800;

Terrain::Terrain(const glm::uvec2& grid, const Texture& texture) {
	std::vector<Vertex> vertices;
	vertices.reserve(VertexCount * VertexCount);
	for (auto i = 0u; i < VertexCount; i++)
		for (auto j = 0u; j < VertexCount; j++) {
			const glm::vec3 position = { (float)j / ((float)VertexCount - 1) * Size, -5.0f, (float)i / ((float)VertexCount - 1) * Size };
			const glm::vec2 texCoords = { (float)j / ((float)VertexCount - 1), (float)i / ((float)VertexCount - 1) };

			vertices.emplace_back(position, texCoords);
		}

	std::vector<std::uint32_t> indices;
	indices.reserve(6 * (VertexCount - 1) * (VertexCount - 1));
	for (auto i = 0u; i < VertexCount - 1; i++) {
		for (auto j = 0u; j < VertexCount -  1; j++) {
			int topLeft = (i * VertexCount) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * VertexCount) + j;
			int bottomRight = bottomLeft + 1;

			indices.emplace_back(topLeft);
			indices.emplace_back(bottomLeft);
			indices.emplace_back(topRight);
			indices.emplace_back(topRight);
			indices.emplace_back(bottomLeft);
			indices.emplace_back(bottomRight);
		}
	}

	rawModel = std::make_unique<RawModel>(vertices, indices);
}
