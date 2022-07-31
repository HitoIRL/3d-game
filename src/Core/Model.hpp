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

class Mesh {
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<std::shared_ptr<Texture>>& textures);
	~Mesh();

	void Draw() const;
private:
	std::uint32_t vao, buffer, indexCount, indicesOffset;
	std::vector<std::shared_ptr<Texture>> textures;
};

class Model {
public:
	Model(std::string_view path);

	void Draw() const;
private:
	void CreateMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> FetchTextures(const aiMaterial* material, aiTextureType type);

	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<std::shared_ptr<Texture>> loadedTextures;
};
