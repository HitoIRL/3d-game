#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <assimp/scene.h>

#include "Texture.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Model {
public:
	Model(std::string_view path);
	~Model();

	void Draw() const;
	void BindVAO(bool state) const;
private:
	void CreateMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> FetchTextures(const aiMaterial* material, aiTextureType type);

	std::uint32_t vao, buffer, indicesOffset;
	std::string directory;
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<Vertex> vertices; // not splitting into meshes limits our buffer size to ~32MB*
	std::vector<std::uint32_t> indices;
};
