#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <assimp/scene.h>

#include "Texture.hpp"

struct Vertex {
	Vertex(const glm::vec3& position, const glm::vec2& texCoords, int texIndex) : position(position), texCoords(texCoords), texIndex(texIndex) {}

	glm::vec3 position;
	glm::vec2 texCoords;
	int texIndex;
};

class RawModel {
public:
	RawModel(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
	~RawModel();

	void Draw() const;
	void BindVAO(bool state) const;
private:
	std::uint32_t vao, vbo, ibo, indicesSize;
};

class Model {
public:
	Model(std::string_view path);

	void Draw() const;

	const std::unique_ptr<RawModel>& GetRawModel() const { return rawModel; }
private:
	void CreateMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> FetchTextures(const aiMaterial* material, aiTextureType type);

	std::unique_ptr<RawModel> rawModel;
	std::string directory;
	std::vector<std::shared_ptr<Texture>> textures;
};
